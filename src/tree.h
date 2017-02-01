#ifndef Tree_H
#define Tree_H

#include <iostream>
#include <list>
using namespace std;

struct node{
    int weight;
    int value;
    node* left;
    node* right;

    node(int _weight = 0, node* _left = NULL, node* _right = NULL) :
        weight(_weight), left(_left), right(_right) {}

    node(int _value, int _weight, node* _left, node* _right) :
        weight(_weight), value(_value), left(_left), right(_right) {}

    bool leaf(){
        return left == NULL && right == NULL;
    }
};

class tree{
    node* root;

public:
    tree(int weight, tree* left, tree* right){
        root = new node(weight);
        root -> left = copy(left -> getRoot());
        root -> right = copy(right -> getRoot());
    }

    tree(int weight = 0) : root(new node(weight)) {}

    tree(int value, int weight) : root(new node(weight)) {
        root -> value = value;
    }

    tree(node& n) {
        root = copy(&n);
    }

    ~tree(){
        erase(root);
    }

    void print(){
        printNode(root);
        cout << endl;
    }

    node* getRoot();

    bool leaf();

    void printTreeList(list<tree*> trees);

    int getValue();

    void setValue(int val);

    int getWeight();

    void setWeight(int val);

    node* left();

    node* right();

private:
    node* copy(node* n);

    void erase(node* n);

    void printNode(node* n);

};

#endif // Tree_H

