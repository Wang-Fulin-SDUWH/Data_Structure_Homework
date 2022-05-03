// @author 201800820139 18数据科学班 王福临
// Created by Wanghaha23333 on 2021/8/8.
// 注：为体现和treeset的区别，这个二叉搜索树保留了重复值，规则是左子树<根结点，右子树>=根结点。

#ifndef CL1_BST_H
#define CL1_BST_H
#include <iostream>
#include <iomanip>


template <class T>
class Visitor{
public:
    virtual void visit(T key)=0;
};

using namespace std;

//二叉搜索树的节点构成的类
template <class T>
class BSTNode{
public:
    T key;            // 关键字(键值)
    BSTNode *left;    // 左孩子
    BSTNode *right;    // 右孩子
    BSTNode *parent;// 父结点

    BSTNode(T value, BSTNode *p, BSTNode *l, BSTNode *r):
            key(value),parent(),left(l),right(r) {}
};


//二叉搜索树的类
template <class T>
class BSTree {
private: // 对类外部不可见的功能函数
    BSTNode<T> *mRoot;    // 根结点
    int len=0;

    // (递归实现)查找"二叉树x"中键值为key的节点
    BSTNode<T>* search(BSTNode<T>* x, T key) const;

    // 将结点(z)插入到二叉树(tree)中
    void add(BSTNode<T>* &tree, BSTNode<T>* z);

    // 将结点(z)插入到二叉树(tree)中，不可重复
    void add2(BSTNode<T>* &tree, BSTNode<T>* z);

    // 查找最小结点：返回tree为根结点的二叉树的最小结点。
    BSTNode<T>* minimum(BSTNode<T>* tree);
    // 查找最大结点：返回tree为根结点的二叉树的最大结点。
    BSTNode<T>* maximum(BSTNode<T>* tree);

    // 找结点(x)的后继结点。即，查找"二叉树中数据值大于该结点"的"最小结点"。
    BSTNode<T>* successor(BSTNode<T> *x);
    // 找结点(x)的前驱结点。即，查找"二叉树中数据值小于该结点"的"最大结点"。
    BSTNode<T>* predecessor(BSTNode<T> *x);

    // 删除二叉树(tree)中的结点(z)，并返回被删除的结点
    BSTNode<T>* remove(BSTNode<T>* &tree, BSTNode<T> *z);

    // 遍历以tree为结点的BST
    void traversal(BSTNode<T> *node, Visitor<T> *visitor);
public:
    /*
     * 对类外部可见的接口列表：
     * size():元素个数
     * isEmpty():是否为空
     * inOrder():中序遍历
     * add(T key):添加元素
     * remove(T key):删除元素
     * contains(T key):是否包含某元素
     */
    // 元素个数
    int size(){
        return len;
    }

    bool isEmpty(){
        return len==0;
    }

    // 判断BST中是否包含某元素
    bool contains(T key);

    // 将结点(key为节点键值)插入到二叉树中
    void add(T key);

    // 将结点(key为节点键值)插入到二叉树中，不可重复
    void add2(T key);

    // 删除结点(key为节点键值)
    void remove(T key);

    // 销毁整棵二叉树
    void clear(){
        mRoot=nullptr;
        len=0;
    }

    // visitor遍历
    void traversal(Visitor<T> *visitor);
};


//其余部分是功能函数的实现

/*
 * 函数功能：带visitor的遍历函数，中序遍历访问结点node为根结点的子树的结点
 * 参数：node:树的结点
 *      visitor:访问器Visitor
 */
template <class T>
void BSTree<T>::traversal(BSTNode<T>* node, Visitor<T> *visitor)
{
    if(node==nullptr)
        return;
    traversal(node->left,visitor);
    visitor->visit(node->key);
    traversal(node->right,visitor);
}

/*
 * 函数功能：带visitor的遍历函数，中序遍历访问整个二叉搜索树的结点
 * 参数：
 *      visitor:访问器Visitor
 */
template <class T>
void BSTree<T>::traversal(Visitor<T> *visitor)
{
    if(visitor==nullptr)
        return;
    traversal(mRoot,visitor);
}

/*
 * 递归找树中是否包含值为key的结点。
 */
template <class T>
bool BSTree<T>::contains(T key)
{
    return _contains(mRoot, key);
}

/*
 * 递归找结点x子树中是否包含值为key的结点。
 */
template <class T>
bool _contains(BSTNode<T>* root,T key)
{
    if(root==NULL)
        return false;
    if(key==root->key)
        return true;
    if(key<root->key)
        return _contains(root->left, key);
    else
        return _contains(root->right, key);
}

/*
 * 找结点x子树中数据为key的结点。
 */
template <class T>
BSTNode<T>* BSTree<T>::search(BSTNode<T>* x, T key) const
{
    if (x==NULL || x->key==key)
        return x;

    if (key < x->key)
        return search(x->left, key);
    else
        return search(x->right, key);
}


/*
 * 找结点x的前驱结点。即，查找"二叉树中数据值小于该结点"的"最大结点"。
 */
template <class T>
BSTNode<T>* BSTree<T>::predecessor(BSTNode<T> *x)
{
    // 如果x存在左孩子，则"x的前驱结点"为 "以其左孩子为根的子树的最大结点"。
    if (x->left != NULL)
        return maximum(x->left);

    // 如果x没有左孩子。则x有以下两种可能：
    // (1) x是"一个右孩子"，则"x的前驱结点"为 "它的父结点"。
    // (2) x是"一个左孩子"，则查找"x的最低的父结点，并且该父结点要具有右孩子"，找到的这个"最低的父结点"就是"x的前驱结点"。
    BSTNode<T>* y = x->parent;
    while ((y!=NULL) && (x==y->left))
    {
        x = y;
        y = y->parent;
    }

    return y;
}

/*
 * 找结点x的后继结点。即，查找"二叉树中数据值大于该结点"的"最小结点"。
 */
template <class T>
BSTNode<T>* BSTree<T>::successor(BSTNode<T> *x)
{
    // 如果x存在右孩子，则"x的后继结点"为 "以其右孩子为根的子树的最小结点"。
    if (x->right != NULL)
        return minimum(x->right);

    // 如果x没有右孩子。则x有以下两种可能：
    // (1) x是"一个左孩子"，则"x的后继结点"为 "它的父结点"。
    // (2) x是"一个右孩子"，则查找"x的最低的父结点，并且该父结点要具有左孩子"，找到的这个"最低的父结点"就是"x的后继结点"。
    BSTNode<T>* y = x->parent;
    while ((y!=NULL) && (x==y->right))
    {
        x = y;
        y = y->parent;
    }

    return y;
}


/*
 * 将结点可重复地插入到二叉树中
 * 参数说明：
 *     tree 二叉树的根结点
 *     z 插入的结点
 */
template <class T>
void BSTree<T>::add(BSTNode<T>* &tree, BSTNode<T>* z)
{
    BSTNode<T> *y = NULL;
    BSTNode<T> *x = tree;

    // 查找z的插入位置
    while (x != NULL)
    {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;
    if (y==NULL)
    {
        tree = z;
        len+=1;
    }
    else if (z->key < y->key)
    {
        y->left = z;
        len+=1;
    }
    else
    {
        y->right = z;
        len+=1;
    }
}

/*
 * 将结点(key为节点键值)可重复地插入到二叉树中
 *
 * 参数说明：
 *     tree 二叉树的根结点
 *     key 插入结点的键值
 */
template <class T>
void BSTree<T>::add(T key)
{
    BSTNode<T> *z=NULL;

    // 如果新建结点失败，则返回。
    if ((z=new BSTNode<T>(key,NULL,NULL,NULL)) == NULL)
        return ;

    add(mRoot, z);
}


/*
 * 将结点不可重复地插入到二叉树中
 * 参数说明：
 *     tree 二叉树的根结点
 *     z 插入的结点
 */

template <class T>
void BSTree<T>::add2(BSTNode<T>* &tree, BSTNode<T>* z)
{
    BSTNode<T> *y = NULL;
    BSTNode<T> *x = tree;
    // 查找z的插入位置
    while (x != NULL)
    {
        y = x;
        if(z->key==x->key)
            return;
        else if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;
    if (y==NULL)
    {
        tree = z;
        len+=1;
    }
    else if (z->key < y->key)
    {
        y->left = z;
        len+=1;
    }
    else
    {
        y->right = z;
        len+=1;
    }
}


/*
 * 将结点(key为节点键值)不可重复地插入到二叉树中
 *
 * 参数说明：
 *     tree 二叉树的根结点
 *     key 插入结点的键值
 */

template <class T>
void BSTree<T>::add2(T key)
{
    BSTNode<T> *z=NULL;

    // 如果新建结点失败，则返回。
    if ((z=new BSTNode<T>(key,NULL,NULL,NULL)) == NULL)
        return ;

    add2(mRoot, z);
}



/*
 * 删除结点(z)，并返回被删除的结点
 *
 * 参数说明：
 *     tree 二叉树的根结点
 *     z 删除的结点
 */
template <class T>
BSTNode<T>* BSTree<T>::remove(BSTNode<T>* &tree, BSTNode<T> *z)
{
    BSTNode<T> *x=NULL;
    BSTNode<T> *y=NULL;

    if ((z->left == NULL) || (z->right == NULL) )
        y = z;
    else
        y = successor(z);

    if (y->left != NULL)
        x = y->left;
    else
        x = y->right;

    if (x != NULL)
        x->parent = y->parent;

    if (y->parent == NULL)
        tree = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    if (y != z)
        z->key = y->key;

    return y;
}

/*
 * 删除结点(z)
 *
 * 参数说明：
 *     key: 被删除的参数
 */
template <class T>
void BSTree<T>::remove(T key)
{
    BSTNode<T> *z, *node;

    if ((z = search(mRoot, key)) != NULL)
    {
        if ( (node = remove(mRoot, z)) != NULL) {
            delete node;
            cout<<"\n删除结点"<<key<<"成功！"<<endl;
            len--;
        }
    }
    else
    {
        cout<<"\n结点"<<key<<"不在集合中，删除无效！"<<endl;
        return;
    }

}


template <class T>
BSTNode<T>* BSTree<T>::maximum(BSTNode<T>* tree)
{
    if (tree == NULL)
        return NULL;

    while(tree->right != NULL)
        tree = tree->right;
    return tree;
}

template <class T>
BSTNode<T>* BSTree<T>::minimum(BSTNode<T>* tree)
{
    if (tree == NULL)
        return NULL;

    while(tree->left != NULL)
        tree = tree->left;
    return tree;
}

#endif //CL1_BST_H
