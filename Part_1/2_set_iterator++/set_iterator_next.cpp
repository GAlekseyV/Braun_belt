#include "test_runner.h"
#include <cassert>
#include <deque>
#include <iostream>

using namespace std;


struct Node
{
  Node(int value, Node *ptr)
    : value(value), parent(ptr)
  {}

  int value;
  Node *left = nullptr;
  Node *right = nullptr;
  Node *parent;
};


class NodeBuilder
{
public:
  Node *CreateRoot(int value)
  {
    nodes.emplace_back(value, nullptr);
    return &nodes.back();
  }

  Node *CreateLeftSon(Node *me, int value)
  {
    assert(me->left == nullptr);
    nodes.emplace_back(value, me);
    me->left = &nodes.back();
    return me->left;
  }

  Node *CreateRightSon(Node *me, int value)
  {
    assert(me->right == nullptr);
    nodes.emplace_back(value, me);
    me->right = &nodes.back();
    return me->right;
  }

private:
  deque<Node> nodes;
};

Node* Minimun(Node* root)
{
  Node* minimum = root;
  while (minimum->left != nullptr) {
    minimum = minimum->left;
  }
  return minimum;
}

Node* Next(Node* node)
{
  if (node->right != nullptr) {
    return Minimun(node->right);
  }
  Node* next = node->parent;
  Node* prev = node;
  while (next != nullptr && prev == next->right) {
    prev = next;
    next = next->parent;
  }
  return next;
}

namespace {

void Test1()
{
  NodeBuilder nbuilder;

  Node *root = nbuilder.CreateRoot(50);
  ASSERT_EQUAL(root->value, 50);

  Node *left_node = nbuilder.CreateLeftSon(root, 2);
  Node *min = nbuilder.CreateLeftSon(left_node, 1);
  Node *right_node = nbuilder.CreateRightSon(left_node, 4);
  ASSERT_EQUAL(min->value, 1);
  ASSERT_EQUAL(right_node->parent->value, 2);

  nbuilder.CreateLeftSon(right_node, 3);
  nbuilder.CreateRightSon(right_node, 5);

  right_node = nbuilder.CreateRightSon(root, 100);
  left_node = nbuilder.CreateLeftSon(right_node, 90);
  nbuilder.CreateRightSon(right_node, 101);

  nbuilder.CreateLeftSon(left_node, 89);
  right_node = nbuilder.CreateRightSon(left_node, 91);

  ASSERT_EQUAL(Next(left_node)->value, 91);
  ASSERT_EQUAL(Next(root)->value, 89);
  ASSERT_EQUAL(Next(min)->value, 2);
  ASSERT_EQUAL(Next(right_node)->value, 100);
}

void TestRootOnly()
{
  NodeBuilder nbuilder;
  Node *root = nbuilder.CreateRoot(42);
  ASSERT(Next(root) == nullptr);
};
} // namespace

int main()
{
  TestRunner trunner;
  RUN_TEST(trunner, Test1);
  RUN_TEST(trunner, TestRootOnly);
  return 0;
}
