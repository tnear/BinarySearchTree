// Classic binary search tree modernized somewhat to use smart pointers and move semantics (academic exercise)
// Tree is templated and includes a few test cases

#pragma once
#include <vector>
#include <string>
#include <cassert>
#include <deque>
#include <memory>

template <typename T>
struct BinaryNode
{
    BinaryNode(const T &value)
        : value(value)
    {}

    std::shared_ptr<BinaryNode<T>> left;
    std::shared_ptr<BinaryNode<T>> right;
    const T value;
};

template <typename T>
class BinarySearchTree
{
public:
    using NodePtr = std::shared_ptr<BinaryNode<T>>;

    void insert(const T &value)
    {
        auto node = m_head;
        auto parent = node;

        // find location to insert
        while (1)
        {
            if (!node)
                break;

            parent = node;
            if (value < node->value)
                node = node->left;
            else
                node = node->right;
        }

        // create new node
        auto newNode = std::make_shared<BinaryNode<T>>(value);

        // move into appropriate location to avoid atomic increment count on shared_ptr
        if (!parent)
            m_head = std::move(newNode); // root
        else if (value < parent->value)
            parent->left = std::move(newNode);
        else
            parent->right = std::move(newNode);
    }

    NodePtr find(const T &value)
    {
        return _find(value, m_head);
    }

    std::vector<T> inOrder()
    {
        // return in-order traversal
        std::vector<T> values;
        _inOrder(m_head, values);
        return values;
    }

    std::vector<int> BreadthFirstSearch()
    {
        // breadth first search using queue to accumulate values in each level of tree
        if (!m_head)
            return {};

        std::vector<T> values;
        std::deque<NodePtr> d = { m_head };

        while (!d.empty())
        {
            auto front = d.front();
            values.push_back(front->value);
            d.pop_front();
            
            if (front->left)
                d.push_back(front->left);

            if (front->right)
                d.push_back(front->right);
        }

        return values;
    }

private:
    void _inOrder(NodePtr &node, std::vector<T> &values)
    {
        if (!node)
            return;

        _inOrder(node->left, values);
        values.push_back(node->value);
        _inOrder(node->right, values);
    }

    NodePtr _find(const T &value, NodePtr &node)
    {
        if (!node)
            return nullptr;
        else if (value == node->value)
            return node;
        else if (value < node->value)
            return _find(value, node->left);
        else
            return _find(value, node->right);
    }

    NodePtr m_head;
};

void testBinaryTree()
{
    // basic test cases for core functionality
    BinarySearchTree<int> bt;
    assert(bt.inOrder() == std::vector<int>({ }));
    assert(bt.BreadthFirstSearch() == std::vector<int>({ }));
    bt.insert(5);
    assert(bt.inOrder() == std::vector<int>({ 5 }));

    bt.insert(10);
    assert(bt.inOrder() == std::vector<int>({ 5, 10 }));

    bt.insert(3);
    assert(bt.inOrder() == std::vector<int>({ 3, 5, 10 }));

    bt.insert(4);
    bt.insert(7);
    bt.insert(12);
    bt.insert(15);
    assert(bt.inOrder() == std::vector<int>({ 3, 4, 5, 7, 10, 12, 15 }));

    auto node = bt.find(5);
    assert(node->value == 5);
    node = bt.find(15);
    assert(node->value == 15);
    node = bt.find(4);
    assert(node->value == 4);
    node = bt.find(1000);
    assert(!node);

    assert(bt.BreadthFirstSearch() == std::vector<int>({ 5, 3, 10, 4, 7, 12, 15 }));
}

void testDataTypes()
{
    // test different templated types including numeric and string
    BinarySearchTree<double> btDouble;
    btDouble.insert(5.1);
    btDouble.insert(4.2);
    btDouble.insert(7.3);
    btDouble.insert(12.4);
    assert(btDouble.inOrder() == std::vector<double>({ 4.2, 5.1, 7.3, 12.4 }));

    BinarySearchTree<std::string> btString;
    btString.insert("abc");
    btString.insert("zyx");
    btString.insert("defg");
    btString.insert("");
    std::vector<std::string> exp = { "", "abc", "defg", "zyx" };
    assert(btString.inOrder() == exp);
}
