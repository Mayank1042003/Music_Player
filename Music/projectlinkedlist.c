#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define the structure for a Song Node in the Linked List
struct Node {
    char *song_name;
    char *artist;
    char *album;
    char *genre;
    float duration;
    struct Node *next;
};

// Playlist Node for Linked List
struct Playlist {
    struct Node *song;
    struct Playlist *next;
};

// Function to create a new Node
struct Node* createNode(char *song_name, char *artist, char *album, char *genre, float duration) {
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    node->song_name = strdup(song_name);
    node->artist = strdup(artist);
    node->album = strdup(album);
    node->genre = strdup(genre);
    node->duration = duration;
    node->next = NULL;
    return node;
}

// Insert song into the linked list in sorted order
struct Node* insertSong(struct Node* head, char *song_name, char *artist, char *album, char *genre, float duration) {
    struct Node* newNode = createNode(song_name, artist, album, genre, duration);

    // If list is empty or the new song should be the first node
    if (!head || strcmp(song_name, head->song_name) < 0) {
        newNode->next = head;
        return newNode;
    }

    // Traverse the list to find the correct position
    struct Node* current = head;
    while (current->next && strcmp(song_name, current->next->song_name) > 0) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
    return head;
}

// Search for a song in the linked list
struct Node* searchSong(struct Node* head, const char *song_name) {
    struct Node* current = head;
    while (current) {
        if (strcmp(current->song_name, song_name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Display a song's details
void displayNode(struct Node *node) {
    if (node != NULL) {
        printf("Song Name: %s\n", node->song_name);
        printf("Artist: %s\n", node->artist);
        printf("Album: %s\n", node->album);
        printf("Genre: %s\n", node->genre);
        printf("Duration: %.2f\n", node->duration);
    }
}

// Add song to playlist
void addToPlaylist(struct Playlist** playlist, struct Node* song) {
    struct Playlist* newEntry = (struct Playlist*) malloc(sizeof(struct Playlist));
    newEntry->song = song;
    newEntry->next = *playlist;
    *playlist = newEntry;
}

// Display the playlist
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

// Shuffle and play songs in the playlist
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

// Display the menu
void displayMenu() {
    printf("\n--- Music Library Menu ---\n");
    printf("1. Search for a song\n");
    printf("2. Add song to playlist\n");
    printf("3. Display playlist\n");
    printf("4. Play songs in shuffle\n");
    printf("5. Exit\n");
}

// Main function
int main() {
    struct Node* head = NULL;
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
        head = insertSong(head, song_name, artist, album, genre, duration);
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
                foundSong = searchSong(head, song_name);
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
                foundSong = searchSong(head, song_name);
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
