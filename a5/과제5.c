#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_APT_ON_ONE_CITY 40
#define MAX_CITY 10
#define MAX_STACK_SIZE 1000

typedef struct adjListNode* adjListPointer;
typedef struct adjListNode {
	int dest;
	int weight;
	adjListPointer next;
} adjListNode;

typedef struct adjList {
	int Nnode;
	adjListPointer* adj_head;
	adjListPointer* adj_tail;
} adjList;

typedef struct cityNode {
	int largest, Napt, Nedge;
	int apt[MAX_APT_ON_ONE_CITY];
} cityNode;

typedef struct stackList* stackPointer;
typedef struct stackList {
	int top;
	int* arr;
} stackList;

typedef struct edgeNode {
	int start, end, weight;
} edgeNode;

int** init_adj_matrix(int num_node);
adjList* init_adj_list(int num_node);
adjListPointer init_adj_list_node(int dest, int weight);
void insert_adj_list(adjList* graph, adjListPointer insert_node, int start_node);
void add_edges(FILE* fp, int** adj_matrix, adjList* adj_list, int num_edges);

int dfs(int** graph, int num_vertices, cityNode* city);

stackPointer init_stack(int size);
int is_blank(stackPointer stack);
void push(stackPointer stack, int element);
int pop(stackPointer stack);

cityNode find_most_apt_city(cityNode* city_list, int n);
edgeNode* find_and_sort_edge(adjList* adj_list, cityNode city);

int get_parent(int* parent, int x);
void union_parent(int* parent, int x, int y);
int find(int* parent, int x, int y);
int kruskal(cityNode city, edgeNode* edges, int total_num_node);

// bonus functions
void print_adj_matrix(int** adj_matrix, int num_node);
void check_adj_list(adjList* adj_list);
void check_city_and_apt(int n, cityNode* city_list);
void print_edges(cityNode city, edgeNode* edges);

/*=======================================================================================================================================*/
void main() {
	FILE* fp = fopen("A.txt", "r");

	int N, E;
	cityNode cities[MAX_CITY];

	fscanf(fp, "%d %d", &N, &E);

	int** graph_matrix = init_adj_matrix(N); // adj matrix �ʱ�ȭ
	adjList* graph_list = init_adj_list(N); // adj list �ʱ�ȭ
	add_edges(fp, graph_matrix, graph_list, E); // edge �߰� (adj matrix, adj list ����)
	//print_adj_matrix(graph_matrix, N); // adj matrix ����� �������� ���
	//check_adj_list(graph_list); // adj list ����� �������� ���

	fclose(fp);

	int num_city = dfs(graph_matrix, N, cities);
	//check_city_and_apt(num_city, cities); // dfs �Ŀ� city�� �ű⿡ ���Ե� apt�� �� �������� Ȯ��

	cityNode has_most_apt_city = find_most_apt_city(cities, num_city);
	edgeNode* edges = find_and_sort_edge(graph_list, has_most_apt_city);
	//print_edges(has_most_apt_city, edges); // ����� ���ĵǾ����� ���
	int mst_weight = kruskal(has_most_apt_city, edges, N);

	printf("%d\n", num_city);
	printf("%d %d %d\n", has_most_apt_city.Napt, has_most_apt_city.Nedge, has_most_apt_city.largest + 1);
	printf("%d\n", mst_weight);
}
/*=======================================================================================================================================*/

// bonus functions
void print_adj_matrix(int** adj_matrix, int num_node) {
	printf("   ");
	for (int i = 0; i < num_node; i++) printf("%2d ", i + 1);
	printf("\n");
	for (int i = 0; i < num_node; i++) {
		printf("%2d ", i + 1);
		for (int j = 0; j < num_node; j++)
			printf("%2d ", adj_matrix[i][j]);
		printf("\n");
	}
}

void check_adj_list(adjList* adj_list) {
	for (int i = 0; i < adj_list->Nnode; i++) {
		adjListPointer cur = adj_list->adj_head[i];
		printf("node %d\n", i + 1);
		for (cur = cur->next; cur != NULL; cur = cur->next) {
			printf("%d %d\n", cur->dest + 1, cur->weight);
		}
		printf("\n");
		free(cur);
	}
}

void check_city_and_apt(int n, cityNode* city_list) {
	printf("number of cities: %d\n\n", n);
	for (int c = 0; c < n; c++) {
		printf("city %d\nlargest apt: %d, number of apts: %d, number of edges: %d\napt list: ",
			c + 1, city_list[c].largest + 1, city_list[c].Napt, city_list[c].Nedge);
		for (int a = 0; a < city_list[c].Napt; a++) {
			printf("%d ", city_list[c].apt[a] + 1);
		}
		printf("\n\n");
	}
}

void print_edges(cityNode city, edgeNode* edges) {
	for (int e = 0; e < city.Nedge; e++) printf("start: %d, end: %d, weight: %d\n",
		edges[e].start + 1, edges[e].end + 1, edges[e].weight);
}