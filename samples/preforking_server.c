#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <sys/wait.h>

int main() 
{
    // Tao socket
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener != -1)
        printf("Socket created: %d\n", listener);

    // Khai bao cau truc dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    // Gan dia chi voi socket
    bind(listener, (struct sockaddr *)&addr, sizeof(addr));
    listen(listener, 5);

    int num_processes = 8;
    char buf[256];

    for (int i = 0; i < num_processes; i++)
        if (fork() == 0)
            while(1)
            {
                // Chờ kết nối
                int client = accept(listener, NULL, NULL);
                printf("New client accepted in process %d: %d\n", client, getpid());

                // Chờ dữ liệu từ client
                int ret = recv(client, buf, sizeof(buf), 0);
                if (ret <= 0)
                    continue;

                // Xử lý dữ liệu, trả lại kết quả cho client
                buf[ret] = 0;
                printf("Received from client %d: %s\n", client, buf);
                send(client, buf, strlen(buf), 0);

                // Đóng kết nối
                close(client);
            }

    wait(NULL);

    return 0;
}