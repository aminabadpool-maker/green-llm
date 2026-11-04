#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GREEN_THRESHOLD 3

struct Node;

typedef struct WhiteEdge {
    struct Node* target;
    float weight;
    int visits;
} WhiteEdge;

typedef struct Pipe {
    struct Node* target;
    int is_green;
} Pipe;

typedef struct Node {
    char word[32];
    int layer;
    Pipe* green_pipes;
    int green_count;
    WhiteEdge* white_edges;
    int white_count;
} Node;

Node* create_node(const char* word, int layer) {
    Node* node = (Node*)malloc(sizeof(Node));
    strncpy(node->word, word, 31);
    node->word[31] = '\0';
    node->layer = layer;
    node->green_pipes = NULL;
    node->green_count = 0;
    node->white_edges = NULL;
    node->white_count = 0;
    return node;
}

void add_white_edge(Node* from, Node* to, float weight) {
    from->white_count++;
    from->white_edges = (WhiteEdge*)realloc(from->white_edges, sizeof(WhiteEdge) * from->white_count);
    from->white_edges[from->white_count - 1].target = to;
    from->white_edges[from->white_count - 1].weight = weight;
    from->white_edges[from->white_count - 1].visits = 0;
}

void solidify_to_green(Node* from, int white_index) {
    Node* target_node = from->white_edges[white_index].target;
    from->green_count++;
    from->green_pipes = (Pipe*)realloc(from->green_pipes, sizeof(Pipe) * from->green_count);
    from->green_pipes[from->green_count - 1].target = target_node;
    from->green_pipes[from->green_count - 1].is_green = 1;
    printf("\n[ارتقا] مسیر %s ===> %s از یال سفید به «لوله سبز» تثبیت شد!\n", from->word, target_node->word);
}

void flow_and_learn(Node* start_node) {
    Node* current = start_node;
    printf("\n=== اجرای جریان و فرایند تثبیت (Learning Phase) ===\n");
    while (current != NULL) {
        printf("[طبقه %d: %s]", current->layer, current->word);
        if (current->green_count > 0) {
            printf(" ===(لوله سبز)===> ");
            current = current->green_pipes[0].target;
        } else if (current->white_count > 0) {
            current->white_edges[0].visits++;
            printf(" ~~~(یال سفید - بازدید: %d/%d)~~> ", current->white_edges[0].visits, GREEN_THRESHOLD);
            if (current->white_edges[0].visits >= GREEN_THRESHOLD) {
                solidify_to_green(current, 0);
            }
            current = current->white_edges[0].target;
        } else {
            current = NULL;
        }
    }
    printf("\n");
}

int main() {
    printf("در حال ساخت نودها و شبکه‌بندی آزمایشی...\n");
    Node* n1 = create_node("I", 0);
    Node* n2 = create_node("learn", 1);
    Node* n3 = create_node("C", 2);

    add_white_edge(n1, n2, 0.8f);
    add_white_edge(n2, n3, 0.9f);

    for (int i = 1; i <= 4; i++) {
        printf("\n--- اجرای جریان شماره %d ---", i);
        flow_and_learn(n1);
    }

    free(n1->white_edges); if(n1->green_pipes) free(n1->green_pipes); free(n1);
    free(n2->white_edges); if(n2->green_pipes) free(n2->green_pipes); free(n2);
    free(n3->white_edges); if(n3->green_pipes) free(n3->green_pipes); free(n3);
    return 0;
}
