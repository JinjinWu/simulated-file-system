/* The functions operate on a linked list of free blocks.  Each node of the
 * list contains the starting location and the length of a free block.
 * 
 */

#include <string.h>
#include <stdlib.h>

#include "free_list.h"

/* Give free space back to the free list.  Since the list is ordered by 
 * location, this function is the same for both algorithms.
 */
void add_free_block(FS *fs, int location, int size) {
    // create new freelist if it doesn't exist
    if (fs->freelist == NULL) {
	Freeblock *head = malloc(size);
	head->offset = location;
	head->length = size;
	head->next = NULL;
	fs->freelist = head;
    } else {
	Freeblock *prev = NULL;
	Freeblock *curr = fs->freelist;
	// create new freeblock
	Freeblock *new_block = malloc(size);
	new_block->offset = location;
	new_block->length = size;
	new_block->next = NULL;

	// iterate to correct position in linked list
	while (location > curr->offset && curr != NULL) {
	    prev = curr;
	    curr = curr->next;
	}
     	// update pointers of modified linked list of freeblocks
	if (prev == NULL) {
	    new_block->next = curr;
	    // update head of linked list
	    fs->freelist = new_block;
	} else {
	    prev->next = new_block;
	    new_block->next = curr;
	}


	// consolidate blocks

	// reset curr variable to head of linked list
	prev = NULL;
	curr = fs->freelist;
	Freeblock *next = curr->next;
	// iterate through linked list looking for adjacent blocks to merge
	while (next != NULL) {
	    if ((curr->offset + curr->length) == next->offset) {
		// create new joint block
		Freeblock *joined_block = malloc(curr->length + next->length);
		joined_block->offset = curr->offset;
		joined_block->length = (curr->length + next->length);

		if (prev != NULL) {
		    prev->next = joined_block;
		} else {
		    //assign new head to pointer in file system
		    fs->freelist = joined_block;
		}

		joined_block->next = next->next;
		// free the memory blocks for two separate blocks
		free(curr);
		free(next);
		// update variables, prev doesn't change in this case
		curr = joined_block;
		next = joined_block->next;

	    // not adjacent blocks so iterate to next nodes
	    } else {
	        prev = curr;
	        curr = next;
	        next = next->next;
	    }
	}

    }

    return;
}

/* Print the contents of the free list */

void show_freelist(FS *fs) {
    Freeblock *curr = fs->freelist;
    printf("Free List\n");

    while(curr != NULL) {
	printf("(offset: %d, length: %d)\n", curr->offset, curr->length);
	curr = curr->next;
    }
    return;
}


/* To be used after the metadata has been read from a file, to rebuild the
 * free list.
 */
void rebuild_freelist(FS *fs) {
    // initialize new freelist
    fs->freelist = NULL;
    add_free_block(fs, METADATA_ENDS, MAX_FS_SIZE);

    int i, j, offset;
    j = -1;
    offset = 0;

    for (i = 0; i < MAXFILES; i++) {
	// if file exists at this index
	if (fs->metadata[i].offset > 0) {
	    printf("Offset: %d Length: %d\n", fs->metadata[i].offset, fs->metadata[i].length);
	    if (fs->metadata[i].offset > offset) {
		if (j > -1) {
		    get_free_block(fs, fs->metadata[i].offset - fs->metadata[j].offset - fs->metadata[j].length);
		}
		get_free_block(fs, fs->metadata[i].length);
	    }
	    offset = fs->metadata[i].offset;
	    j = i;
	}
    }

    j = -1;
    offset = 0;

    for (i = 0; i < MAXFILES; i++) {
	if (fs->metadata[i].offset > 0) {
	    printf("%d\n", i);
	    if (fs->metadata[i].offset < offset) {
		//get_free_block(fs, fs->metadata[i].length);
		continue;
	    }
	    if (j > -1) {
		add_free_block(fs, offset + fs->metadata[j].length, fs->metadata[i].offset - offset - fs->metadata[j].length);
	    }
            offset = fs->metadata[i].offset;
            j = i;
	}
    }

    return;
}
