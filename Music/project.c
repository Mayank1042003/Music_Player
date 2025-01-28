#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define the structure for the AVL Tree Node
struct Node {
    char *song_name;
    char *artist;
    char *album;
    char *genre;
    float duration;
    int height;
    struct Node *left;
    struct Node *right;
};

// Playlist Node for Linked List
struct Playlist {
    struct Node *song;
    struct Playlist *next;
};

// AVL Tree Functions
int getHeight(struct Node *node) {
    return node ? node->height : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

struct Node* createNode(char *song_name, char *artist, char *album, char *genre, float duration) {
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    node->song_name = strdup(song_name);
    node->artist = strdup(artist);
    node->album = strdup(album);
    node->genre = strdup(genre);
    node->duration = duration;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

// Right Rotate
struct Node* rightRotate(struct Node *y) {
    struct Node *x = y->left;
    struct Node *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

// Left Rotate
struct Node* leftRotate(struct Node *x) {
    struct Node *y = x->right;
    struct Node *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

// Balance Factor
int getBalance(struct Node *node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

// Insert into AVL Tree
struct Node* insertNode(struct Node* node, char *song_name, char *artist, char *album, char *genre, float duration) {
    if (node == NULL)
        return createNode(song_name, artist, album, genre, duration);

    if (strcmp(song_name, node->song_name) < 0)
        node->left = insertNode(node->left, song_name, artist, album, genre, duration);
    else if (strcmp(song_name, node->song_name) > 0)
        node->right = insertNode(node->right, song_name, artist, album, genre, duration);
    else
        return node;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    if (balance > 1 && strcmp(song_name, node->left->song_name) < 0)
        return rightRotate(node);

    if (balance < -1 && strcmp(song_name, node->right->song_name) > 0)
        return leftRotate(node);

    if (balance > 1 && strcmp(song_name, node->left->song_name) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(song_name, node->right->song_name) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

// Search for a Song
struct Node* search(struct Node* root, const char *song_name) {
    if (root == NULL || strcmp(root->song_name, song_name) == 0)
        return root;

    if (strcmp(song_name, root->song_name) < 0)
        return search(root->left, song_name);

    return search(root->right, song_name);
}

// Display a Song's Details
void displayNode(struct Node *node) {
    if (node != NULL) {
        printf("Song Name: %s\n", node->song_name);
        printf("Artist: %s\n", node->artist);
        printf("Album: %s\n", node->album);
        printf("Genre: %s\n", node->genre);
        printf("Duration: %.2f\n", node->duration);
    }
}

// Add Song to Playlist (Linked List)
void addToPlaylist(struct Playlist** playlist, struct Node* song) {
    struct Playlist* newEntry = (struct Playlist*) malloc(sizeof(struct Playlist));
    newEntry->song = song;
    newEntry->next = *playlist;
    *playlist = newEntry;
}

// Display Playlist
void displayPlaylist(struct Playlist* playlist) {
    struct Playlist* current = playlist;
    if (current == NULL) {
        printf("Playlist is empty.\n");
        return;
    }
    while (current != NULL) {
        displayNode(current->song);
        printf("\n");
        current = current->next;
    }
}

// Shuffle and Play Songs in Playlist
void shuffleAndPlay(struct Playlist* playlist) {
    struct Playlist* songs[100];
    int count = 0;
    struct Playlist* current = playlist;
    while (current != NULL) {
        songs[count++] = current;
        current = current->next;
    }
    srand(time(0));
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        struct Playlist* temp = songs[i];
        songs[i] = songs[j];
        songs[j] = temp;
    }
    printf("Playing songs in shuffle:\n");
    for (int i = 0; i < count; i++) {
        displayNode(songs[i]->song);
        printf("\n");
    }
}

// Main function with Menu
void displayMenu() {
    printf("\n--- Music Library Menu ---\n");
    printf("1. Search for a song\n");
    printf("2. Add song to playlist\n");
    printf("3. Display playlist\n");
    printf("4. Play songs in shuffle\n");
    printf("5. Exit\n");
}

int main() {
    struct Node* root = NULL;
    struct Playlist* playlist = NULL;
    int choice;
    char song_name[100];

    // Load songs from file
    FILE *file = fopen("songs.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open file\n");
        return 1;
    }
    
    char artist[100], album[100], genre[50];
    float duration;
    
    while (fscanf(file, " %99[^,], %99[^,], %99[^,], %49[^,], %f", song_name, artist, album, genre, &duration) == 5) {
        root = insertNode(root, song_name, artist, album, genre, duration);
    }
    fclose(file);

    // Menu loop
    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // to clear newline after choice

        struct Node* foundSong;
        switch (choice) {
            case 1:
                printf("Enter song name to search: ");
                fgets(song_name, sizeof(song_name), stdin);
                song_name[strcspn(song_name, "\n")] = '\0';
                foundSong = search(root, song_name);
                if (foundSong) {
                    printf("Song found:\n");
                    displayNode(foundSong);
                } else {
                    printf("Song not found.\n");
                }
                break;
            case 2:
                printf("Enter song name to add to playlist: ");
                fgets(song_name, sizeof(song_name), stdin);
                song_name[strcspn(song_name, "\n")] = '\0';
                foundSong = search(root, song_name);
                if (foundSong) {
                    addToPlaylist(&playlist, foundSong);
                    printf("Song added to playlist.\n");
                } else {
                    printf("Song not found.\n");
                }
                break;
            case 3:
                printf("Your playlist:\n");
                displayPlaylist(playlist);
                break;
            case 4:
                shuffleAndPlay(playlist);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);

    return 0;
}
