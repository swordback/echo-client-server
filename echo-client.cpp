#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>

using namespace std;

#define max_len 1024

// https://const-human.tistory.com/12

void read_socket(int sock) {
    while (true) {
        char read_msg[max_len];
        int cl_read = read(sock, read_msg, max_len - 1);
        if (cl_read == -1) {
            cout << "error in read" << endl;
            exit(-4);
        }
        cout << read_msg << endl;
    }
}

void write_socket(int sock) {
    while (true) {
        char write_msg[max_len];
        scanf("%s", write_msg);
        write(sock, write_msg, sizeof(write_msg));
    }
}
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Argument error!" << endl;
        exit(-1);
    }

    struct sockaddr_in sv_addr;
    int cl_sock, cl_cn, cl_read;
    char write_msg[max_len];
    char read_msg[max_len];
    
    cl_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (cl_sock == -1) {
        cout << "error in making socket" << endl;
        exit(-2);
    }

    memset(&sv_addr, 0, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    sv_addr.sin_port = htons(atoi(argv[2]));
    
    cl_cn = connect(cl_sock, (struct sockaddr*)&sv_addr, sizeof(sv_addr));

    if (cl_cn == -1){
        cout << "error in connect" << endl;
        exit(-3);
    }
    thread th1 = thread(read_socket, cl_sock);
    thread th2 = thread(write_socket, cl_sock);
    th1.join();
    th2.join();

}