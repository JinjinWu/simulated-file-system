# simulated-file-system
Read metadata and allocate memory using simulated blocks of a free list.
Create a free list using a linked list. 
Allocate memory using written first fit and best fit algorithms.
Split large blocks of memory and coalesce blocks freed and add into correct spot in free list according to address.
Restore an existing file using metadata and data of a file and construct a free list from this information.

Usage:
- Type "make" on command line to compile files. (ffsim and bfsim)
- > ./ffsim [test_file]
- > ./ffsim ./testfiles/test_sample
- > ./bfsim [test_file]
- > ./bfsim ./testfiles/test_sample
