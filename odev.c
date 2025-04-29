#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100

typedef struct User {
    int id;
    struct User* friends[MAX_USERS];
    int friend_count;
} User;

typedef struct RBNode {
    int id;
    User* user;
    struct RBNode* left;
    struct RBNode* right;
    int color;
} RBNode;

User* userList[MAX_USERS];
int userCount = 0;

User* createUser(int id) {
    User* newUser = (User*)malloc(sizeof(User));
    newUser->id = id;
    newUser->friend_count = 0;
    userList[userCount++] = newUser;
    return newUser;
}

User* getUser(int id) {
    for (int i = 0; i < userCount; i++) {
        if (userList[i]->id == id) return userList[i];
    }
    return NULL;
}

void addFriendship(User* a, User* b) {
    a->friends[a->friend_count++] = b;
    b->friends[b->friend_count++] = a;
}

void readDataFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Dosya acilamadi.\n");
        exit(1);
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "USER", 4) == 0) {
            int id;
            sscanf(line, "USER %d", &id);
            createUser(id);
        } else if (strncmp(line, "FRIEND", 6) == 0) {
            int id1, id2;
            sscanf(line, "FRIEND %d %d", &id1, &id2);
            User* u1 = getUser(id1);
            User* u2 = getUser(id2);
            if (u1 && u2) addFriendship(u1, u2);
        }
    }

    fclose(file);
}

void dfsDistance(User* user, int targetDepth, int currentDepth, int* visited) {
    if (currentDepth == targetDepth) {
        printf("Kullanici %d mesafe %d'de\n", user->id, targetDepth);
        return;
    }

    visited[user->id] = 1;
    for (int i = 0; i < user->friend_count; i++) {
        if (!visited[user->friends[i]->id]) {
            dfsDistance(user->friends[i], targetDepth, currentDepth + 1, visited);
        }
    }
    visited[user->id] = 0;
}

void commonFriends(User* u1, User* u2) {
    printf("%d ve %d ortak arkadaslari:\n", u1->id, u2->id);
    for (int i = 0; i < u1->friend_count; i++) {
        for (int j = 0; j < u2->friend_count; j++) {
            if (u1->friends[i]->id == u2->friends[j]->id) {
                printf("- %d\n", u1->friends[i]->id);
            }
        }
    }
}

void dfsCommunity(User* user, int* visited) {
    visited[user->id] = 1;
    printf("%d ", user->id);
    for (int i = 0; i < user->friend_count; i++) {
        if (!visited[user->friends[i]->id]) {
            dfsCommunity(user->friends[i], visited);
        }
    }
}

int countInfluence(User* user, int* visited) {
    visited[user->id] = 1;
    int count = 1;
    for (int i = 0; i < user->friend_count; i++) {
        if (!visited[user->friends[i]->id]) {
            count += countInfluence(user->friends[i], visited);
        }
    }
    return count;
}

RBNode* createRBNode(User* user) {
    RBNode* node = (RBNode*)malloc(sizeof(RBNode));
    node->id = user->id;
    node->user = user;
    node->left = node->right = NULL;
    node->color = 1;
    return node;
}

RBNode* insertRBTree(RBNode* root, User* user) {
    if (root == NULL) return createRBNode(user);

    if (user->id < root->id)
        root->left = insertRBTree(root->left, user);
    else if (user->id > root->id)
        root->right = insertRBTree(root->right, user);

    return root;
}

User* searchRBTree(RBNode* root, int id) {
    if (!root) return NULL;
    if (root->id == id) return root->user;
    else if (id < root->id) return searchRBTree(root->left, id);
    else return searchRBTree(root->right, id);
}

int main() {
    readDataFile("veriseti.txt");

    printf("\n--- Kullanici 101 icin mesafe 2 arkadaslar ---\n");
    int visited[MAX_USERS] = {0};
    User* u = getUser(101);
    dfsDistance(u, 2, 0, visited);

    printf("\n--- 101 ve 104 ortak arkadas analizi ---\n");
    commonFriends(getUser(101), getUser(104));

    printf("\n--- Topluluklar (baglantili kullanicilar) ---\n");
    memset(visited, 0, sizeof(visited));
    for (int i = 0; i < userCount; i++) {
        if (!visited[userList[i]->id]) {
            printf("Topluluk: ");
            dfsCommunity(userList[i], visited);
            printf("\n");
        }
    }

    printf("\n--- Etki Alani Hesaplama ---\n");
    for (int i = 0; i < userCount; i++) {
        memset(visited, 0, sizeof(visited));
        int inf = countInfluence(userList[i], visited) - 1;
        printf("Kullanici %d, %d kisiye etki ediyor\n", userList[i]->id, inf);
    }

    printf("\n--- Red-Black Tree Testi ---\n");
    RBNode* tree = NULL;
    for (int i = 0; i < userCount; i++) {
        tree = insertRBTree(tree, userList[i]);
    }

    int testID = 104;
    User* found = searchRBTree(tree, testID);
    if (found)
        printf("Kullanici %d agacta bulundu.\n", found->id);
    else
        printf("Kullanici %d agacta yok.\n", testID);

    return 0;
}