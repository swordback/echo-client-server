#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

#define max_len 1024
#define max_client 20

char bc_recv_msg[max_len];
int cl_sock[max_client];
mutex mtx1;

void read_write_client(int cl_sock, int mode) {
    int sv_read;
    char recv_msg[max_len];
    while (true) {
        sv_read = read(cl_sock, recv_msg, max_len - 1);
        cout << recv_msg << endl;
        if (mode == 1) {
            write(cl_sock, recv_msg, (strlen(recv_msg) + 1) * sizeof(char));
        }
        else {
            char okay_msg[] = "okay";
            write(cl_sock, okay_msg, sizeof(okay_msg) + 1);
        }
    }
}

/*
void read_client(int cl_sock, int mode) {
    int sv_read;
    char recv_msg[max_len];
    while(true) {
        mtx1.lock();
        sv_read = read(cl_sock, recv_msg, max_len - 1);
        cout << recv_msg << endl;
        for (int num1 = 0; num1 < max_client; num1++) {
            if (cl_sock[num1] > 0) {
                thread 
            }
        }
        mtx1.unlock();
    }
}

void write_client(int cl_sock, int mode) {
    int num1;
}
*/

int main(int argc, char* argv[]) {
    int mode = 0;
    if (argc > 4 || argc < 2) {
        cout << "Argument error!" << endl;
        exit(-1);
    }
    if (argc == 3) {
        if (strcmp(argv[2], "-e") == 0) {
            mode = 1;
        }
        else {
            exit(-1);
        }
    }
    if (argc == 4) {
        if (strcmp(argv[2], "-e") == 0) {
            if (strcmp(argv[3] , "-b") == 0) {
                mode = 2;
            }
            else {
                exit(-1);
            }
        }
        else if (strcmp(argv[2], "-b") == 0) {
            if (strcmp(argv[3] , "-e") == 0) {
                mode = 2;
            }
            else {
                exit(-1);
            }
        }
        else {
            exit(-1);
        }
    }
    cout << mode << endl;
    struct sockaddr_in sv_addr;
    struct sockaddr_in cl_addr;
    int cl_sock_cnt = 0;
    int sv_sock, sv_bind, sv_listen;
    vector<thread> workers;

    sv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sv_sock == -1) {
        cout << "socket() error" << endl;
        exit(-1);
    }
    char ip[] = "192.168.10.2";
    memset(&sv_addr, 0, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sv_addr.sin_port = htons(atoi(argv[1]));

    sv_bind = bind(sv_sock, (struct sockaddr*)&sv_addr, sizeof(sv_addr));
    if (sv_bind == -1) {
        cout << "bind() error" << endl;
        exit(-2);
    }

    while (true) {
        
        sv_listen = listen(sv_sock, 10);
        if (sv_listen == -1) {
            exit(-3);
            cout << "listen() error" << endl;
        }

        socklen_t cl_addr_size = sizeof(cl_addr);
        cl_sock[cl_sock_cnt] = accept(sv_sock, (struct sockaddr*)&cl_addr, &cl_addr_size);
        if (cl_sock[cl_sock_cnt] == -1) {
            exit(-4);
            cout << "accept() error" << endl;
        }
        thread th1 = thread(read_write_client, cl_sock[cl_sock_cnt], mode);
        cl_sock_cnt = (cl_sock_cnt + 1) % max_client;
        th1.join();
    }
}