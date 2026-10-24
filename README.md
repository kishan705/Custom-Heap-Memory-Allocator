# Custom Heap Memory Allocator (C, POSIX)

A custom dynamic memory allocation library for Unix-based systems (Linux/macOS), implementing custom `malloc()` and `free()` functions from scratch using system calls.

This project was built to understand the low-level mechanics of heap memory management, pointer arithmetic, and the memory constraints critical to Embedded Systems and Real-Time Operating Systems (RTOS).

## 🚀 Features

*   **Custom `my_malloc` and `my_free`:** Replaces the standard C library dynamic memory functions.
*   **System-Level Memory Requests:** Interfaces directly with the operating system via the `sbrk()` system call to expand the data segment.
*   **8-Byte Memory Alignment:** Ensures all allocated blocks are padded to 8-byte boundaries to optimize CPU cache lines and prevent hardware-level alignment faults (especially critical on ARM/Embedded processors).
*   **Automatic Block Coalescing:** The `my_free()` function actively scans and merges adjacent free blocks in the heap, effectively preventing external memory fragmentation.
*   **Benchmarking Suite:** Includes a testing suite to validate coalescing logic and measure allocation speed across 50,000 operations.

## 🧠 System Architecture

At its core, this allocator manages the heap as an **explicit doubly linked list** of memory blocks. 

When a user requests memory, the allocator prepends a hidden `block_meta` metadata header right before the returned memory address. This header tracks the block's size and availability. 

```c
struct block_meta {
    size_t size;             // Size of the data block
    int is_free;             // 1 if free, 0 if in use
    struct block_meta *next; // Pointer to the next memory block
};
```

When `my_free()` is called, pointer arithmetic `(struct block_meta*)ptr - 1` is used to step backward in memory, retrieve the metadata, and mark the block as available for future coalescing or reuse.

## 🛠️ Build and Run Instructions

This project includes a `Makefile` to streamline the build process and silence macOS deprecation warnings for `sbrk()`.

1. **Clone the repository and navigate to the directory:**
   ```bash
   git clone <your-repo-link>
   cd custom_allocator
   ```

2. **Compile the library and benchmarking suite:**
   ```bash
   make
   ```

3. **Run the benchmark:**
   ```bash
   ./benchmark
   ```

**Expected Output:**
```text
Starting Memory Allocator Benchmark...
Custom Allocator Time: 5.500174 seconds
Benchmark Complete! Coalescing successful.
```

## 📈 Future Optimizations

While functional, this allocator uses a linear **First-Fit** algorithm which operates in O(N) time. To make this production-ready for an RTOS environment, future improvements include:

1.  **Segregated Free Lists:** Replacing the linear linked list with buckets of pre-sized memory blocks to achieve O(1) allocation time.
2.  **Thread Safety (Mutexes):** Wrapping the allocation and deallocation logic in hardware mutexes to prevent race conditions during OS context switches.
3.  **Modern System Calls:** Upgrading `sbrk()` to `mmap()` to handle memory in distinct, isolated pages rather than a contiguous heap block.
