#include "Heap.hpp"
#include "classes.h"

int main()
{
    Group g1("a");
    Group g2("b");
    Group g3("c");
    Group g4("d");
    Group g5("e");
    Group g6("f");
    Group g7("g");
    Group g8("h");
    Group g9("i");
    Group g10("j");
    FibonacciHeap<Group> heap;
    Node<Group>* temp, * node1 = nullptr;
    std::cout << "INSERT 10 NODES\n\n";
    temp = heap.Insert(g1);
    temp = heap.Insert(g2);
    temp = heap.Insert(g3);
    temp = heap.Insert(g4);
    temp = heap.Insert(g5);
    node1 = temp;
    temp = heap.Insert(g6);
    temp = heap.Insert(g7);
    temp = heap.Insert(g8);
    temp = heap.Insert(g9);
    temp = heap.Insert(g10);

    heap.Print();

    std::cout << "\n\nEXTRACT MIN\n\n";
    heap.ExtractMin();
    heap.Print();

    std::cout << "\n\nDECREASE e to a\n\n";
    heap.DecreaseKey(node1, g1);
    heap.Print();

    std::cout << "\n\nEXTRACT MIN\n\n";
    heap.ExtractMin();
    heap.Print();

    return 0;
}

