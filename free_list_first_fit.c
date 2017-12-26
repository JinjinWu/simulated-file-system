#include <string.h>
#include <stdlib.h>

#include "free_list.h"

/* Implement the first fit algorithm to find free space for the
   simulated file data.
 */

int get_free_block(FS *fs, int size) {

    Freeblock *prev = NULL;
    Freeblock *curr = fs->freelist;

    while (curr != NULL) {
	if (curr->length >= size) {
	    int index = curr->offset;
	    curr->offset += size;
	    curr->length -= size;
	    // block is same size, so remove it
	    if (curr->length == 0) {
		if (prev != NULL) {
		    // skip the curr node we're about to delete
		    prev->next = curr->next;
		} else {
		    fs->freelist = curr->next;
		}
		// free the memory for the now empty block
		free(curr);
	    }
	    return index;
	}
	// iterate to next node in freelist
	prev = curr;
	curr = curr->next;
    }

    return -1;
}

