# WheelOfFate - Circular Linked List Game
Author:Bilal ASLAN

A C++ implementation of a hybrid data structure combining a **circular linked list**, **stack**, and **hash table**.  
It allows "spinning" through nodes with deterministic but dynamic skip values and supports rollback.

## Features
- Insert and delete nodes
- Spin through nodes with dynamic skips
- Rollback to previous positions
- Trace spin movements for debugging

## Usage
```cpp
#include "wheel_of_fate.cpp"

int main() {
    WheelOfFate wheel;
    wheel.insert(10);
    wheel.insert(25);
    wheel.spinUntil(5);
    wheel.printCurrent();
    wheel.rollback(2);
    wheel.printCurrent();
    return 0;
}
