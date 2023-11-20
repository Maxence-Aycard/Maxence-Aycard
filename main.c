#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct Graph {
    int num_vertices;
    Node* adjacency_list[MAX_VERTICES];
} Graph;

void add_edge(Graph* graph, int u, int v) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->vertex = v;
    new_node->next = graph->adjacency_list[u];
    graph->adjacency_list[u] = new_node;

    new_node = (Node*)malloc(sizeof(Node));
    new_node->vertex = u;
    new_node->next = graph->adjacency_list[v];
    graph->adjacency_list[v] = new_node;
}

void welsh_powell(Graph* graph, int colors[]) {
    // Trie les sommets en fonction de leur degré décroissant
    int sorted_nodes[MAX_VERTICES];
    for (int i = 0; i < graph->num_vertices; ++i) {
        sorted_nodes[i] = i;
    }

    for (int i = 0; i < graph->num_vertices - 1; ++i) {
        for (int j = i + 1; j < graph->num_vertices; ++j) {
            if (graph->adjacency_list[sorted_nodes[i]]->next < graph->adjacency_list[sorted_nodes[j]]->next) {
                int temp = sorted_nodes[i];
                sorted_nodes[i] = sorted_nodes[j];
                sorted_nodes[j] = temp;
            }
        }
    }

    // Initialise les couleurs et les sommets coloriés
    int colored_nodes[MAX_VERTICES] = {0};

    // Parcours les sommets triés
    for (int i = 0; i < graph->num_vertices; ++i) {
        int node = sorted_nodes[i];

        // Si le sommet n'est pas encore colorié, lui attribue une couleur
        if (!colored_nodes[node]) {
            int color = 1;
            Node* current = graph->adjacency_list[node];

            while (current != NULL) {
                int neighbour = current->vertex;
                if (colored_nodes[neighbour] && colors[neighbour] == color) {
                    color++;
                    current = graph->adjacency_list[node];
                } else {
                    current = current->next;
                }
            }

            colors[node] = color;
            colored_nodes[node] = 1;
        }
    }
}

void find_non_adjacent_vertices(Graph* graph, int colors[]) {
    printf("Sommets ne pouvant pas être adjacents :\n");

    for (int i = 0; i < graph->num_vertices; ++i) {
        Node* current = graph->adjacency_list[i];

        while (current != NULL) {
            int neighbour = current->vertex;

            if (colors[i] == colors[neighbour]) {
                printf("%d et %d\n", i, neighbour);
            }

            current = current->next;
        }
    }
}

int main() {
    Graph graph;
    int colors[MAX_VERTICES] = {0};

    // Exemple de graphe
    graph.num_vertices = 5;
    for (int i = 0; i < MAX_VERTICES; ++i) {
        graph.adjacency_list[i] = NULL;
    }

    add_edge(&graph, 0, 1);
    add_edge(&graph, 0, 2);
    add_edge(&graph, 1, 2);
    add_edge(&graph, 1, 3);
    add_edge(&graph, 2, 3);
    add_edge(&graph, 3, 4);

    welsh_powell(&graph, colors);

    printf("Coloriage des sommets :\n");
    for (int i = 0; i < graph.num_vertices; ++i) {
        printf("Sommet %d : Couleur %d\n", i, colors[i]);
    }

    find_non_adjacent_vertices(&graph, colors);

    return 0;
}

