#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pipe {
    struct Node* target;   
    int is_green;          
} Pipe;

typedef struct Node {
    char word[32];         
    int layer;             
    Pipe* pipes;           
    int pipe_count;        
} Node;

Node* create_node(const char* word, int layer) {
    Node* node = (Node*)malloc(sizeof(Node));
    strncpy(node->word, word, 31);
    node->word[31] = '\0';
    node->layer = layer;
    node->pipes = NULL;
    node->pipe_count = 0;
    return node;
}

void connect_green_pipe(Node* from, Node* to) {
    from->pipe_count++;
    from->pipes = (Pipe*)realloc(from->pipes, sizeof(Pipe) * from->pipe_count);
    from->pipes[from->pipe_count - 1].target = to;
    from->pipes[from->pipe_count - 1].is_green = 1; 
}

void flow_data(Node* start_node) {
    Node* current = start_node;
    printf("\n=== بررسی جریان در طبقات ساختمان توپولوژیک ===\n");
    
    while (current != NULL) {
        printf("[طبقه %d: %s]", current->layer, current->word);
        
        if (current->pipe_count > 0 && current->pipes[0].is_green) {
            printf(" ===(لوله سبز)===> ");
            current = current->pipes[0].target;
        } else {
            current = NULL;
        }
    }
    printf("\n=== رسیدن جریان به انتهای لوله‌کشی ساختمان ===\n\n");
}

int main() {
    printf("در حال ساخت فنداسیون ساختمان و لوله‌کشی لایه‌ای...\n");

    Node* n1 = create_node("I", 0);       
    Node* n2 = create_node("go", 1);      
    Node* n3 = create_node("to", 1);      
    Node* n4 = create_node("school", 2);  

    connect_green_pipe(n1, n2); 
    connect_green_pipe(n2, n3); 
    connect_green_pipe(n3, n4); 

    flow_data(n1);

    free(n1->pipes); free(n1);
    free(n2->pipes); free(n2);
    free(n3->pipes); free(n3);
    free(n4->pipes); free(n4);

    return 0;
}
