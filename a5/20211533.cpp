#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

#define MAX_APT_ON_ONE_CITY 40
#define MAX_CITY 10
#define MAX_STACK_SIZE 1000

typedef struct stackList* stackPointer;
typedef struct stackList {
    int top;
    int* arr;
} stackList;

void add_edges(std::ifstream& fin, std::vector<std::vector<int>>& adj_matrix, int num_edges);

std::vector<int> dfs(std::vector<std::vector<int>> graph, int num_vertices, std::vector<std::pair<int, int>>& cities);

stackPointer init_stack(int size);
int is_blank(stackPointer stack);
void push(stackPointer stack, int element);
int pop(stackPointer stack);
void free_stack(stackPointer stack);

int find_most_apt_city(std::vector<std::pair<int, int>> cities);
std::vector<std::pair<int, std::pair<int, int>>> find_and_sort_edge(std::vector<std::vector<int>> graph, std::vector<int> visited, int city);

void uni(std::vector<int>& set, int x, int y);
int find(std::vector<int>& set, int x);
int kruskal(int city_apt_largest, std::vector<std::pair<int, std::pair<int, int>>> edges);

/*
    1. DFS(깊이우선탐색) 알고리즘 함수 작성 시 recursive가 아닌 iterative function으로 작성하고, 반드시 스택을 이용한다.
    2. 스택 사용 시 전역변수를 사용하지 않고 주어진 구조체를 이용하며, init_stack, is_blank, push, pop 함수를 작성한다.
    3. Graph 표현 방식은 adjacent matrix 또는 adjacent list 중 원하는 방법을 이용하고, 둘 중 하나만 사용하거나 모두 사용해도 무방하다.
    4. MST(최소신장트리)를 구할 때에는 Kruskal 알고리즘을 사용하도록 하며, 해당 알고리즘 구현시 사용할 정렬방식은 자유롭게 선택하도록 한다.
    5. 출력 조건에 맞는 내용만 출력하도록 한다. 불필요한 내용이 포함되어 출력된 경우 감점될 수있다.
    6. 1~5 조건을 만족하는 선에서 주어진 template 코드(과제5.c)의 모든 내용은 자유롭게 수정이 가능하며 (main함수 포함), 단순 참고용이다.
*/

/*=======================================================================================================================================*/
int main() {

    std::ifstream fin("A.txt");

    int N, E;
    fin >> N >> E;

    std::vector<std::vector<int>> graph_matrix;
    for (int i = 0; i < N; i++) {
        std::vector<int> row(N, -1);
        graph_matrix.push_back(row);
    }

    add_edges(fin, graph_matrix, E);

    fin.close();

    // vector<pair<아파트수, 최고가아파트>> cities
    std::vector<std::pair<int, int>> cities;
    // vector<도시 번호> visited (index=아파트 번호)
    std::vector<int> visited = dfs(graph_matrix, N, cities);

    // 아파트수와 최고가아파트가 가장 큰 index 찾기
    int city = find_most_apt_city(cities);

    int city_apt_num = cities[city].first;
    int city_apt_largest = cities[city].second;

    // city에 해당하는 아파트의 edge들을 찾고 정렬하여 반환
    // vector<pair<이동시간, pair<출발 아파트, 도착 아파트>>>
    std::vector<std::pair<int, std::pair<int, int>>> edges = find_and_sort_edge(graph_matrix, visited, city);

    // kruskal 알고리즘으로 mst 구성
    int mst_weight = kruskal(city_apt_largest, edges);

    int city_size = cities.size();
    int edge_size = edges.size();

    std::cout << city_size << "\n";
    std::cout << city_apt_num << " " << edge_size << " " << city_apt_largest + 1 << "\n";
    std::cout << mst_weight << "\n";

    return 0;
}
/*=======================================================================================================================================*/

void add_edges(std::ifstream& fin, std::vector<std::vector<int>>& adj_matrix, int num_edges) {
    int s, e, w;

    for (int i = 0; i < num_edges; i++) {
        fin >> s >> e >> w;

        adj_matrix[s - 1][e - 1] = w;
        //adj_matrix[e][s] = w;
    }
}

/**
 * @brief graph에 대하여 DFS를 수행한 후 도시 정보를 갱신하고 각 아파트가 속하는 도시에 관한 정보를 반환한다.
 * 
 * @param graph 그래프
 * @param num_vertices V 
 * @param cities vector<pair<아파트수, 최고가아파트>> cities
 * @return std::vector<int> vector<도시 번호> visited (index=아파트 번호)
 */
std::vector<int> dfs(std::vector<std::vector<int>> graph, int num_vertices, std::vector<std::pair<int, int>>& cities) {

    stackPointer stack = init_stack(MAX_STACK_SIZE);

    std::vector<int> visited(num_vertices, -1);

    // 모든 vertex에 대하여
    for (int i = 0; i < num_vertices; i++) {

        // 이미 방문한 vertex이면 건너뛰기
        if (visited[i] >= 0) {
            continue;
        }

        // 새로운 도시
        cities.push_back({ 0, 0 });
        push(stack, i);

        while (!is_blank(stack)) {

            // stack에서 pop
            int apt = pop(stack);

            // 이미 방문한 vertex이면 건너뛰기
            if (visited[apt] >= 0) continue;

            // 현재 vertex에 도시 할당
            visited[apt] = cities.size() - 1;

            // 도시 정보 갱신
            cities[cities.size() - 1].first++;
            cities[cities.size() - 1].second = std::max(cities[cities.size() - 1].second, apt);

            for (int j = 0; j < num_vertices; j++) {

                // 방문 가능한 모든 vertex를 stack에 추가
                if (graph[apt][j] > 0 || graph[j][apt] > 0) {
                    push(stack, j);
                }

            }

        }

    }

    free_stack(stack);

    return visited;
}

stackPointer init_stack(int size) {
    stackPointer stack = new stackList;
    stack->top = 0;
    stack->arr = new int[size];
    return stack;
}
int is_blank(stackPointer stack) {
    return stack->top == 0;
}
void push(stackPointer stack, int element) {
    stack->arr[stack->top++] = element;
}
int pop(stackPointer stack) {
    return stack->arr[--stack->top];
}
void free_stack(stackPointer stack) {
    delete[] stack->arr;
    delete stack;
}

/**
 * @brief 아파트 수와 최고가 아파트가 최댓값인 city의 index를 반환한다.
 * 
 * @param cities vector<pair<아파트수, 최고가아파트>> cities
 * @return int (아파트 수, 최고가 아파트)가 max인 city의 index
 */

int find_most_apt_city(std::vector<std::pair<int, int>> cities) {

    int most_apt_city = 0;
    auto most_apt_city_info = cities[most_apt_city];

    for (int i = 0; i < cities.size(); i++) {

        if (most_apt_city_info.first < cities[i].first || (most_apt_city_info.first == cities[i].first && most_apt_city_info.second < cities[i].second)) {
            most_apt_city = i;
            most_apt_city_info = cities[i];
        }

    }

    return most_apt_city;

}

/**
 * @brief 주어진 도시의 아파트들의 간선을 추출한 뒤 정렬하여 반환한다.
 * 
 * @param graph 그래프
 * @param visited 각 아파트의 도시 정보
 * @param city 추출할 도시
 * @return std::vector<std::pair<int, std::pair<int, int>>> vector<pair<이동시간, pair<출발 아파트, 도착 아파트>>>
 */
std::vector<std::pair<int, std::pair<int, int>>> find_and_sort_edge(std::vector<std::vector<int>> graph, std::vector<int> visited, int city) {

    std::vector<std::pair<int, std::pair<int, int>>> edges;

    int size = visited.size();

    for (int i = 0; i < size; i++) {

        if (visited[i] == city) {

            for (int j = 0; j < size; j++) {

                if (graph[i][j] < 0) continue;

                edges.push_back({ graph[i][j], {i, j} });

            }

        }

    }

    sort(edges.begin(), edges.end());

    return edges;

}

/**
 * @brief set x와 set y를 union 한다.
 * 
 * @param set disjoint set vector (ref)
 * @param x set x
 * @param y set y
 */
void uni(std::vector<int>& set, int x, int y) {
    x = find(set, x);
    y = find(set, y);
    set[y] = x;
}

/**
 * @brief x의 parent를 검색하고 검색 중 발견한 모든 element에 대해 set을 일치시킨다.
 * 
 * @param set disjoint set vector (ref)
 * @param x element to search
 * @return int set number of x
 */
int find(std::vector<int>& set, int x) {
    if (set[x] == x) {
        return x;
    }
    else {
        return set[x] = find(set, set[x]);
    }
}

/**
 * @brief kruskal 알고리즘에 의해 mst를 구성하고 weight를 반환한다.
 * 
 * @param city_apt_largest 최고가 아파트
 * @param edges 간선 정보들
 * @return int weight
 */
int kruskal(int city_apt_largest, std::vector<std::pair<int, std::pair<int, int>>> edges) {

    std::vector<int> set(city_apt_largest+1);
    for (int i = 0; i < set.size(); i++) {
        set[i] = i;
    }

    int mst_weight = 0;

    int s = edges.size();

    // 가중치가 작은 간선부터 모든 간선에 대하여
    for (int i = 0; i < s; i++) {

        int w = edges[i].first;
        int s = edges[i].second.first;
        int e = edges[i].second.second;

        // set이 동일하면 사이클을 형성하므로 건너뛰기
        if (find(set, s) == find(set, e)) continue;

        // set이 다르면 해당 간선을 선택하고 set 갱신
        uni(set, s, e);

        mst_weight += w;

    }

    return mst_weight;

}