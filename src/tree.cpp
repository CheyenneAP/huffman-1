#ifndef Tree_CPP
#define Tree_CPP

#include "tree.h"

node* tree::getRoot(){
    return root;
}

bool tree::leaf(){
    return root == NULL ||
           (root != NULL && root -> left == NULL && root -> right == NULL);
}

void tree::printTreeList(list<tree*> trees){
    for (list<tree*>::iterator it = trees.begin();it != trees.end();it++){
        (*it) -> print();
        cout << endl;
    }
}

int tree::getValue(){
    return getRoot() -> value;
}

void tree::setValue(int val){
    getRoot() -> value = val;
}

int tree::getWeight(){
    return getRoot() -> weight;
}

void tree::setWeight(int val){
    getRoot() -> weight = val;
}

node* tree::left(){
    if(getRoot() == NULL)
        return NULL;
    return getRoot() -> left;
}

node* tree::right(){
    if(getRoot() == NULL)
        return NULL;
    return getRoot() -> right;
}

node* tree::copy(node* n){
    if(n == NULL)
        return NULL;

    return new node(n -> value,
                    n -> weight,
                    copy(n -> left),
                    copy(n -> right));
}

void tree::erase(node* n){
    if(n != NULL){
        erase(n -> left);
        erase(n -> right);
        delete n;
    }
}

void tree::printNode(node* n){
    if(n != NULL){
        cout << '(' << n -> weight << ' ';
        //if(n -> leaf())
        //    cout << '[' << (char)n -> value << "] ";
        printNode(n -> left);
        cout << ' ';
        printNode(n -> right);
        cout << ')';
    }else{
        cout << 'x';
    }
}

#endif // Tree_CPP
