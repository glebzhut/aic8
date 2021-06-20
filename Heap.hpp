#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>

template <class T>
class FibonacciHeap;

//Class for implementation node of heap
template <typename T>
class Node 
{
private:
    T value;
    Node* parent;
    Node* left;
    Node* right;
    Node* child;
    bool mark{};
    int degree{};

public:
    //Constructors
    Node() 
    {
        value = {};
        mark = false;
        parent = child = nullptr;
        left = right = this;
        degree = 0;
    }
    explicit Node(T newValue) : Node() 
    {
        value = newValue;
    }

    //Destructor
    ~Node() = default;

    //Output in console
    void Print(int countTabs, Node<T>* head) const 
    {
        for (int i = 0; i < countTabs; i++)
            std::cout << '\t';
        std::cout <<"Value: "<< value << "; Degree: " << degree << "; Mark: " << mark;
        if (child != nullptr)
        {
            std::cout << std::endl;
            child->Print(countTabs + 1, child);
        }
        if (left != head)
        {
            std::cout << std::endl;
            left->Print(countTabs, head);
        }
    }

    //Function for clearing node
    void Clear(Node<T>* head) 
    {
        if (this != nullptr) 
        {
            if (child != nullptr)
                child->Clear(child);
            if (left != head)
                left->Clear(head);
            delete this;
        }
    }

    //Insertion node between nodes
    void InsertBetween(Node<T>* _left, Node<T>* _right) 
    {
        _left->right = _right->left = this;
        this->right = _right;
        this->left = _left;
    }

    //Deletion node
    void ExtractBetween() 
    {
        left->right = right;
        right->left = left;
        left = right = nullptr;
    }

    //Overloaded operator for output
    friend std::ostream& operator<< (std::ostream& out, const Node& node) 
    {
        out << node.value;
        return out;
    }

    friend FibonacciHeap<T>;
};

//Class for implementation Fiboncñi Heap
template <typename T>
class FibonacciHeap 
{
private:
    Node<T>* min;
    int count_nodes{};

    //Dividing heap
    void Cut(Node<T>* son, Node<T>* parent) 
    {
        if (son == son->right)
            parent->child = nullptr;
        else
            parent->child = son->right;

        parent->degree--;

        son->parent = nullptr;
        son->ExtractBetween();
        son->InsertBetween(min, min->right);
        son->mark = false;
    }

    void CascadingCut(Node<T>* node) 
    {
        Node<T>* parent = node->parent;
        if (parent != nullptr) 
        {
            if (node->mark == false)
                node->mark = true;
            else 
            {
                Cut(node, parent);
                CascadingCut(parent);
            }
        }
    }

    //Function for recover heap properties
    void Consolidate() 
    {
        int size = round(std::log(count_nodes)) + 2;
        auto array = new Node<T> *[size];
        for (int i = 0; i < size; i++)
            array[i] = nullptr;
        Node<T>* temp = min, * right_sibling = nullptr;
        do 
        {
            right_sibling = temp->right;
            Node<T>* x = temp;
            int degree = x->degree;
            while (array[degree] != nullptr) 
            {
                Node<T>* y = array[degree];
                if (x->value > y->value) 
                {
                    auto z = x;
                    x = y;
                    y = z;
                }
                Link(y, x);
                array[degree] = nullptr;
                degree++;
            }
            array[degree] = x;

            while (min->parent != nullptr)
                min = min->parent;

            temp = right_sibling;
        } while (temp != min);

        for (int i = 0; i < size; i++) 
            if (array[i] != nullptr) 
                if (min->value > array[i]->value)
                    min = array[i];
    }

    //Recover decreasing of nodes
    void Link(Node<T>* lower, Node<T>* higher) 
    {
        lower->ExtractBetween();
        if (higher->child == nullptr) 
        {
            higher->child = lower;
            lower->right = lower->left = lower;
        }
        else lower->InsertBetween(higher->child, higher->child->right);

        lower->parent = higher;
        higher->degree++;
        lower->mark = false;
    }

    [[nodiscard]] Node<T> Minimum()
    {
        return min;
    }

public:
    //Constructors
    FibonacciHeap() : min(nullptr), count_nodes(0) {};

    explicit FibonacciHeap(T value) : FibonacciHeap() 
    {
        min = new Node<T>(value);
    }

    //Destructor
    ~FibonacciHeap() 
    {
        if (min != nullptr)
            min->Clear(min);
    }

    //Output in console
    void Print() const 
    {
        min->Print(0, min);
        std::cout << std::endl;
        std::cout << "Count nodes = " << count_nodes << '\n';
    }

    //Unio 2 heaps
    void UnionHeaps(FibonacciHeap<T>* first, FibonacciHeap<T>* second) 
    {
        if (first->min == nullptr && second->min == nullptr)
            return;
        if (first->min == nullptr) 
        {
            min = second->min;
            count_nodes = second->count_nodes;
            return;
        }
        if (second->min == nullptr) 
        {
            min = first->min;
            count_nodes = first->count_nodes;
            return;
        }
        min = std::min(first->min, second->min);

        Node<T>* rightSiblingFirst = first->min->right;
        Node<T>* leftSiblingSecond = second->min->left;

        first->min->right = second->min;
        second->min->left = first->min;
        rightSiblingFirst->left = leftSiblingSecond;
        leftSiblingSecond->right = rightSiblingFirst;

        count_nodes = first->count_nodes + second->count_nodes;

        second->min = nullptr;
    }

    //Removing the smallest element
    void ExtractMin() 
    {
        Node<T>* head = min;
        if (head != nullptr) 
        {
            if (head->child != nullptr) 
            {
                Node<T>* temp = head->child, * rightSibling = nullptr;
                do 
                {
                    rightSibling = temp->right;
                    temp->parent = nullptr;
                    temp->ExtractBetween();
                    temp->InsertBetween(min->left, min);
                    temp = rightSibling;
                } while (temp != head->child);
            }


            if (head == head->right)
                min = nullptr;
            else 
            {
                min = head->right;
                head->ExtractBetween();
                Consolidate();
            }
            count_nodes--;
        }
        head->child = nullptr;
        head->Clear(head);
    }

    //Changing node value
    void DecreaseKey(Node<T>* node, T newValue) 
    {
        if (newValue > node->value)
            return;
        node->value = newValue;
        Node<T>* parent = node->parent;
        if (parent != nullptr && node->value < parent->value) 
        {
            Cut(node, parent);
            CascadingCut(parent);
        }
        if (node->value < min->value)
            min = node;
    }

    //Deletion current node
    void DeleteNode(Node<T>* node) 
    {
        //DecreaseKey(node, -INF);
        ExtractMin();
    }

    //Insertion new element
    Node<T>* Insert(T value) 
    {
        auto node = new Node<T>(value);
        return Insert(node);
    }

    Node<T>* Insert(Node<T>* node) 
    {
        count_nodes++;
        if (min == nullptr) 
        {
            min = node;
            return node;
        }
        if (min->left == min)
        {

            min->left = node;
            min->right = node;
            node->right = min;
            node->left = min;

        }
        else 
        {
            Node<T>* left_sibling = min->left;

            left_sibling->right = node;
            min->left = node;
            node->right = min;
            node->left = left_sibling;
        }
        if (node->value < min->value)
            min = node;
        return node;
    }
};