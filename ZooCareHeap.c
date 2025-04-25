#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* Author: Zayne Dockery
* Course: CS1
* Assignment: Optional Assignment; Zoo Care Heap
* Date: Spring 2025
* Professor: Ali
*/

// Animal structure to include criticalLevel
typedef struct {
    char name[100];
    int criticalLevel;
} Animal;

// Heap structure to manage animals
typedef struct {
    int capacity;       // Maximum number of animals
    int size;           // Current number of animals
    Animal* animal;
} AnimalHeap;

// Heap with a given capacity
AnimalHeap* initializeHeapWithSize(int capacity) {
    AnimalHeap* heap = (AnimalHeap*)malloc(sizeof(AnimalHeap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->animal = (Animal*)malloc(capacity * sizeof(Animal));
    return heap;
}

// Swap utility function to switch two animals in heap
void swapAnimal(Animal* a, Animal* b) {
    Animal temp = *a;
    *a = *b;
    *b = temp;
}

// Heapify function for max-heap
void heapifyMax(AnimalHeap* heap, int i) {
    int largest = i;
    int left = 2 * i + 1;               // Left child index is 2*i + 1 of the parent
    int right = 2 * i + 2;              // Right child index is 2*i + 2 of the parent

    // Compare critical levels to find largest in heap
    if (left < heap->size && heap->animal[left].criticalLevel > heap->animal[largest].criticalLevel) {
        largest = left;
    }

    if (right < heap->size && heap->animal[right].criticalLevel > heap->animal[largest].criticalLevel) {
        largest = right;
    }

    // If largest is not root, swap and continue heapifying
    if (largest != i) {
        swapAnimal(&heap->animal[i], &heap->animal[largest]);
        heapifyMax(heap, largest);
    }
}

// Insert an animal into the max-heap
void insertAnimal(AnimalHeap* heap, char* name, int criticalLevel) {
    if (heap->size == heap->capacity) {
        printf("Heap is full\n");
        return;
    }

    int i = heap->size++;
    heap->animal[i].criticalLevel = criticalLevel;
    strcpy(heap->animal[i].name, name);

    // Move the new animal up to maintain max-heap
    while (i != 0 && heap->animal[(i - 1) / 2].criticalLevel < heap->animal[i].criticalLevel) {
        swapAnimal(&heap->animal[i], &heap->animal[(i - 1) / 2]);
        i = (i - 1) / 2;
    }

    printf("Inserted: %s, Critical Level: %d\n", name, criticalLevel);
}

// Treat the next animal (highest critical level)
void treatNextAnimal(AnimalHeap* heap) {
    if (heap->size <= 0) {
        printf("No animals to treat\n");
        return;
    }

    Animal treatedAnimal = heap->animal[0];         // Get the most critical animal, usually at the root
    heap->animal[0] = heap->animal[--heap->size];   // Move last animal to root
    heapifyMax(heap, 0);

    printf("Treating: %s, Critical Level: %d\n", treatedAnimal.name, treatedAnimal.criticalLevel);
}

// Peek the most critical animal
void peekMostCritical(AnimalHeap* heap) {
    if (heap->size <= 0) {
        printf("No animals in the heap\n");
        return;
    }

    printf("Most Critical: %s, Critical Level: %d\n", heap->animal[0].name, heap->animal[0].criticalLevel);
}

// Print all animals in the heap
void printAllAnimals(AnimalHeap* heap) {
    printf("Remaining animals in care:\n");
    for (int i = 0; i < heap->size; i++) {
        printf("%s, Critical Level: %d\n", heap->animal[i].name, heap->animal[i].criticalLevel);
    }
}

// Main function to read from file and execute operations
int main(int argc, char* argv[]) {
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        printf("Error: file not found\n");
        return 1;
    }

    AnimalHeap* maxHeap = initializeHeapWithSize(15);
    char name[100];
    int criticalLevel;

    // Read animals from file and insert into heap
    while (fscanf(file, "%s %d", name, &criticalLevel) != EOF) {
        insertAnimal(maxHeap, name, criticalLevel);
    }
    fclose(file);

    // Treat the top two critical animals
    treatNextAnimal(maxHeap);
    treatNextAnimal(maxHeap);

    // Print remaining animals
    printAllAnimals(maxHeap);

    // Free allocated memory
    free(maxHeap->animal);
    free(maxHeap);

    return 0;
}