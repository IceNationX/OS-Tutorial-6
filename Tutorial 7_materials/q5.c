#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

typedef struct {
    char name[50];
    int priority;
    int pid;
    int runtime;
} Process;

typedef struct node {
    Process process;
    struct node *next;
} Node;

Node* head = NULL;
Node* tail = NULL;

void add_process(Process p) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->process = p;
    new_node->next = NULL;

    if (head == NULL) {
        head = new_node;
        tail = new_node;
    } else {
        tail->next = new_node;
        tail = new_node;
    }
}

Process delete_name(char* name) {
    Node* temp = head;
    Node* prev = NULL;

    while (temp != NULL) {
        if (strcmp(temp->process.name, name) == 0) {
            if (prev == NULL) {
                head = temp->next;
            } else {
                prev->next = temp->next;
            }
            Process p = temp->process;
            free(temp);
            return p;
        }
        prev = temp;
        temp = temp->next;
    }

    Process empty = {"", 0, 0, 0};
    return empty;
}

void execute_process(Process p) {
    pid_t pid = fork();

    if (pid == 0) {
        execl("./process", "process", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        p.pid = pid;
        sleep(p.runtime);
        kill(pid, SIGINT);
        waitpid(pid, NULL, 0);
        printf("Name: %s, Priority: %d, PID: %d, Runtime: %d\n", p.name, p.priority, p.pid, p.runtime);
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    FILE *file = fopen("processes_q5.txt", "r");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char name[50];
    int priority, runtime;
    while (fscanf(file, "%[^,],%d,%d\n", name, &priority, &runtime) != EOF) {
        Process p = {"", priority, 0, runtime};
        strcpy(p.name, name);
        add_process(p);
    }
    fclose(file);

    Node* temp = head;
    while (temp != NULL) {
        if (temp->process.priority == 0) {
            Process p = delete_name(temp->process.name);
            execute_process(p);
        }
        temp = temp->next;
    }

    temp = head;
    while (temp != NULL) {
        Process p = delete_name(temp->process.name);
        execute_process(p);
        temp = temp->next;
    }

    return 0;
}
