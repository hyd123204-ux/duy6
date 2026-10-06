#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Node {
    int key;
    struct Node* left;
    struct Node* right;
    int height;
};

struct Node* createNode(int k) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = k;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

int getMax(int a, int b) {
    return (a > b) ? a : b;
}

int getHeight(struct Node* n) {
    return n ? n->height : 0;
}

int getBalanceFactor(struct Node* n) {
    return n ? getHeight(n->left) - getHeight(n->right) : 0;
}

struct Node* rightRotate(struct Node* y) {
    struct Node* x = y->left;
    struct Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = getMax(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = getMax(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

struct Node* leftRotate(struct Node* x) {
    struct Node* y = x->right;
    struct Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = getMax(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = getMax(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

struct Node* insertBST(struct Node* root, int key, long long* compCount, int* inserted) {
    if (!root) {
        *inserted = 1;
        return createNode(key);
    }

    (*compCount)++;
    if (key < root->key) {
        root->left = insertBST(root->left, key, compCount, inserted);
    }
    else if (key > root->key) {
        root->right = insertBST(root->right, key, compCount, inserted);
    }
    else {
        *inserted = 0;
    }
    return root;
}

struct Node* insertAVL(struct Node* root, int key, long long* compCount, int* inserted) {
    if (!root) {
        *inserted = 1;
        return createNode(key);
    }

    (*compCount)++;
    if (key < root->key) {
        root->left = insertAVL(root->left, key, compCount, inserted);
    }
    else if (key > root->key) {
        root->right = insertAVL(root->right, key, compCount, inserted);
    }
    else {
        *inserted = 0;
        return root;
    }

    root->height = 1 + getMax(getHeight(root->left), getHeight(root->right));
    int balance = getBalanceFactor(root);

    if (balance > 1 && key < root->left->key)
        return rightRotate(root);

    if (balance < -1 && key > root->right->key)
        return leftRotate(root);

    if (balance > 1 && key > root->left->key) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && key < root->right->key) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

int getTreeHeight(struct Node* root) {
    if (!root) return 0;
    return 1 + getMax(getTreeHeight(root->left), getTreeHeight(root->right));
}

int searchBST(struct Node* root, int key, int* compCount) {
    struct Node* curr = root;
    while (curr) {
        (*compCount)++;
        if (key == curr->key) return 1;
        else if (key < curr->key) curr = curr->left;
        else curr = curr->right;
    }
    return 0;
}

int main() {
    srand((unsigned int)time(NULL));

    int inputData[100];
    printf("=== 생성된 100개의 난수 (0 ~ 1000) ===\n");
    for (int i = 0; i < 100; ++i) {
        inputData[i] = rand() % 1001;
        printf("%d%s", inputData[i], (i == 99 ? "" : ", "));
    }
    printf("\n\n");

    int arr[100];
    int arrSize = 0;
    long long arrCompCount = 0;

    for (int i = 0; i < 100; ++i) {
        int val = inputData[i];
        int found = 0;
        for (int j = 0; j < arrSize; ++j) {
            arrCompCount++;
            if (arr[j] == val) {
                found = 1;
                break;
            }
        }
        if (!found) {
            arr[arrSize++] = val;
        }
    }

    struct Node* bstRoot = NULL;
    long long bstCompCount = 0;
    for (int i = 0; i < 100; ++i) {
        int inserted = 0;
        bstRoot = insertBST(bstRoot, inputData[i], &bstCompCount, &inserted);
    }

    struct Node* avlRoot = NULL;
    long long avlCompCount = 0;
    for (int i = 0; i < 100; ++i) {
        int inserted = 0;
        avlRoot = insertAVL(avlRoot, inputData[i], &avlCompCount, &inserted);
    }

    printf("저장된 서로 다른 값의 수 : %d\n", arrSize);
    printf("\n[생성 과정 비교 횟수]\n");
    printf("배열 비교 횟수 : %lld\n", arrCompCount);
    printf("BST 비교 횟수   : %lld\n", bstCompCount);
    printf("AVL 비교 횟수 : %lld\n", avlCompCount);

    printf("\n[자료구조 구조 정보]\n");
    printf("배열 길이 : %d\n", arrSize);
    printf("BST 높이   : %d\n", getTreeHeight(bstRoot));
    printf("AVL 높이   : %d\n", getTreeHeight(avlRoot));

    int searchKeys[50];
    printf("\n=== 생성된 50개의 탐색 대상 ===\n");
    for (int i = 0; i < 50; ++i) {
        searchKeys[i] = rand() % 1001;
        printf("%d%s", searchKeys[i], (i == 49 ? "" : ", "));
    }
    printf("\n\n총 탐색 횟수 : 50\n\n");

    long long seqTotalComp = 0;
    long long bstSearchTotalComp = 0;
    long long avlSearchTotalComp = 0;

    for (int i = 0; i < 50; ++i) {
        int key = searchKeys[i];

        int seqComp = 0;
        int seqFound = 0;
        for (int j = 0; j < arrSize; ++j) {
            seqComp++;
            if (arr[j] == key) {
                seqFound = 1;
                break;
            }
        }
        seqTotalComp += seqComp;

        int bstComp = 0;
        int bstFound = searchBST(bstRoot, key, &bstComp);
        bstSearchTotalComp += bstComp;

        int avlComp = 0;
        int avlFound = searchBST(avlRoot, key, &avlComp);
        avlSearchTotalComp += avlComp;

        printf("탐색 대상 값 : %d\n", key);
        printf("순차 탐색 - 결과: %s, 비교 횟수: %d\n", seqFound ? "탐색 성공 (Found)" : "탐색 실패 (Not Found)", seqComp);
        printf("BST 탐색  - 결과: %s, 비교 횟수: %d\n", bstFound ? "탐색 성공 (Found)" : "탐색 실패 (Not Found)", bstComp);
        printf("AVL 탐색  - 결과: %s, 비교 횟수: %d\n", avlFound ? "탐색 성공 (Found)" : "탐색 실패 (Not Found)", avlComp);
        printf("----------------------------------------\n");
    }

    printf("\n[최종 요약]\n");
    printf("순차 탐색 (Sequential Search)\n");
    printf("총 비교 횟수   : %lld\n", seqTotalComp);
    printf("평균 비교 횟수 : %.2f\n", (double)seqTotalComp / 50.0);

    printf("\nBST 탐색 (BST Search)\n");
    printf("총 비교 횟수   : %lld\n", bstSearchTotalComp);
    printf("평균 비교 횟수 : %.2f\n", (double)bstSearchTotalComp / 50.0);

    printf("\nAVL 탐색 (AVL Search)\n");
    printf("총 비교 횟수   : %lld\n", avlSearchTotalComp);
    printf("평균 비교 횟수 : %.2f\n", (double)avlSearchTotalComp / 50.0);

    return 0;
}