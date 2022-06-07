#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/* 아래 구조체 제외하고 전역변수 사용 금지
전역변수 사용 시 0점 처리 */
typedef struct threadedTree* threadedPointer;
typedef struct threadedTree {
    char data;
    short int leftThread, rightThread;
    threadedPointer leftChild, rightChild;
} threadedTree;

/* 구현할 함수들
함수의 반환형이나 parameter는 수정 가능하지만, 함수명은 변경 불가
함수명 변경 시 해당 함수 미구현으로 간주하고 점수 미부여 */
threadedPointer initTree(FILE* fp); // 직접 구현
threadedPointer insucc(threadedPointer tree); // 교재 확인
threadedPointer inpredec(threadedPointer tree); // 직접 구현
void insertRight(threadedPointer parent, threadedPointer child, char data); // 교재 확인
void insertLeft(threadedPointer parent, threadedPointer child, char data); // 직접 구현
threadedPointer tinorderSearch(threadedPointer tree, char data); // 직접 구현
void tinorder(threadedPointer tree); // 교재 확인
void tpreorder(threadedPointer tree); // 직접 구현

void main() {
    /* main 함수는 수정할 수 없음
    main 함수 수정 시 0점 처리 */
    FILE* fp = fopen("input.txt", "r");
    threadedPointer my_tree = initTree(fp);
    tinorder(my_tree);
    printf("\n");
    tpreorder(my_tree->leftChild);
    printf("\n");
    fclose(fp);
    return;
}

threadedPointer initTree(FILE* fp) {

    threadedPointer root = (threadedPointer) malloc(sizeof(threadedTree));
    root->leftChild = root;
    root->leftThread = 1;
    root->rightChild = root;
    root->rightThread = 1;

    threadedPointer search = root;

    char m, c1, c2;
    while(fscanf(fp, " %c", &m)) {

        if(m=='I') { // insert

            fscanf(fp, "%c %c", &c1, &c2);

            threadedPointer child = (threadedPointer) malloc(sizeof(threadedTree));
            child->data = c1;

            if(c2=='L') { // left insert
                insertLeft(search, child, c1);
            } else if(c2=='R') {
                insertRight(search, child, c1);
            } else continue;

        } else if (m=='S') {

            fscanf(fp, "%c", &c1);

            search = tinorderSearch(root, c1);

        } else break;

    }

}

threadedPointer insucc(threadedPointer tree) {
    /* find the inorder successor of tree in a threaded binary tree */
    threadedPointer temp;
    temp = tree->rightChild;
    if (!tree->rightThread)
        while (!temp->leftThread)
            temp = temp->leftChild;
    return temp;
}

threadedPointer inpredec(threadedPointer tree) {
    /* find the inorder predecessor of tree in a threaded binary tree */
    threadedPointer temp;
    temp = tree->leftChild;
    if (!tree->leftThread)
        while (!temp->rightThread)
            temp = temp->rightChild;
    return temp;
}

void insertRight(threadedPointer parent, threadedPointer child, char data) {
    /* insert child as the right child of parent in a threaded binary tree */
    threadedPointer temp;
    child->rightChild = parent->rightChild;
    child->rightThread = parent->rightThread;
    child->leftChild = parent;
    child->leftThread = 1;
    parent->rightChild = child;
    parent->rightThread = 0;
    if (!child->rightThread) {
        temp = insucc(child);
        temp->leftChild = child;
    }
}
void insertLeft(threadedPointer parent, threadedPointer child, char data) {
    /* insert child as the left child of parent in a threaded binary tree */
    threadedPointer temp;
    child->leftChild = parent->leftChild;
    child->leftThread = parent->leftThread;
    child->rightChild = parent;
    child->rightThread = 1;
    parent->leftChild = child;
    parent->leftThread = 0;
    if (!child->leftThread) {
        temp = insucc(child);
        temp->rightChild = child;
    }
}

threadedPointer tinorderSearch(threadedPointer tree, char data) {
    /* traverse the threaded binary tree inorder */
    threadedPointer temp = tree;
    for ( ; ; ) {
        temp = insucc(temp);
        if (temp == tree) break;
        if (tree->data == data) return temp;
    }
}

void tinorder(threadedPointer tree) {
    /* traverse the threaded binary tree inorder */
    threadedPointer temp = tree;
    for ( ; ; ) {
        temp = insucc(temp);
        if (temp == tree) break;
        printf("%3c", temp->data);
    }
}

void tpreorder(threadedPointer tree) {
    /* traverse the threaded binary tree preorder */
    threadedPointer temp = tree;
    for ( ; ; ) {
        temp = inpredec(temp);
        if (temp == tree) break;
        printf("%3c", temp->data);
    }
}