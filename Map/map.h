//
// 201800820139 18级数据科学班 王福临 Map接口的C++实现
// Created by Wanghaha23333 on 2021/8/8.
// Map的实现相当于是用<class K, class V>的模板重写了二叉搜索树里面的<class T>
//

#ifndef CL1_MAP_H
#define CL1_MAP_H
#include <iostream>
#include <iomanip>
#include <queue>

//visitor模板类，被main函数中重新定义的类所继承，纯虚函数visit在类外部被定义，供用户选择个性化的使用遍历元素的方法。
template <class K, class V>
class Visitor{
public:
    bool stop;
    virtual void visit(K key,V value)=0;
};

using namespace std;


//Map的结点构成的类
template <class K, class V>
class MapNode{
public:
    K key;            // 关键字(键值)
    V value;
    MapNode *left;    // 左孩子
    MapNode *right;    // 右孩子
    MapNode *parent;// 父结点
    MapNode(K key_in,V value_in,MapNode *l,MapNode *r):key(key_in),value(value_in),left(l),right(r){}//初始化结点
};


//二叉搜索树的类
template <class K,class V>
class TreeMap {
private: // 对类外部不可见的功能函数设置为私有
    MapNode<K,V> *mRoot;    // 根结点
    int len=0;

    // (递归实现)查找"二叉树x"中键值为key的节点
    MapNode<K,V>* get(MapNode<K,V>* x, K key) const;

    // 将结点(z)插入到二叉树(tree)中，不可重复
    V add(MapNode<K,V>* &tree, MapNode<K,V>* z);

    // 查找最小结点：返回tree为根结点的二叉树的最小结点。
    MapNode<K,V>* minimum(MapNode<K,V>* tree);
    // 查找最大结点：返回tree为根结点的二叉树的最大结点。
    MapNode<K,V>* maximum(MapNode<K,V>* tree);

    // 找结点(x)的后继结点。即，查找"二叉树中key值大于该结点"的"最小结点"。
    MapNode<K,V>* successor(MapNode<K,V> *x);
    // 找结点(x)的前驱结点。即，查找"二叉树中key值小于该结点"的"最大结点"。
    MapNode<K,V>* predecessor(MapNode<K,V> *x);

    // 删除二叉树(tree)中的结点(z)，并返回被删除的结点
    MapNode<K,V>* remove(MapNode<K,V>* &tree, MapNode<K,V> *z);

    // 判断两个Value类型是否相等的函数，考虑了NULL的情况
    bool valEquals(V v1,V v2){
        return v1==NULL?v2==NULL:v1==v2;
    }

    // 遍历以tree为结点的BST
    void traversal(MapNode<K,V> *node, Visitor<K,V> *visitor);
public:
    /*
     * 对类外部可见的接口列表：
     * int size():元素个数
     * bool isEmpty():是否为空
     * void clear():清空
     * V get(K key):根据Key值查找对应的Value
     * V add(K key):根据键值添加元素，如果key已存在，则返回被覆盖的值；key不存在，则返回新添加的值。
     * V remove(K key):根据键值删除元素
     * bool containsKey(K key):是否包含某键
     * bool containsValue(V value):是否包含某值
     * void traversal(Visitor<K,V> visitor):遍历
     * 在类外部实现的函数均做了注释说明。
     */

    // 元素个数
    int size(){
        return len;
    }

    //是否为空
    bool isEmpty(){
        return len==0;
    }

    // 销毁整棵二叉树
    void clear(){
        mRoot=nullptr;//指向根结点的指针直接置为空指针
        len=0;//大小置0
    };

    // 将结点(key为节点键值)插入到二叉树中
    V add(K key,V value);

    // (递归实现)查找"二叉树"中键值为key的节点
    V get(K key);

    // 删除结点(key为节点键值)
    V remove(K key);

    // 判断Map中是否包含键
    bool containsKey(K key){
        return get(key)!=NULL;
    }

    // 判断Map中是否包含值，用层序遍历找
    bool containsValue(V value){
        if(mRoot==nullptr)
            return false;
        queue<MapNode<K,V>*> q0;
        q0.push(mRoot);
        while(!q0.empty())
        {
            MapNode<K,V> *node0=q0.front();
            if(valEquals(node0->value,value))
                return true;
            if(node0->left!=nullptr)
                q0.push(node0->left);
            if(node0->right!=nullptr)
                q0.push(node0->right);
            q0.pop();//队头元素出队
        }
        return false;
    }

    // visitor遍历
    void traversal(Visitor<K,V> *visitor);
};



//其余部分是功能函数的实现。

/*
 * 函数功能：带visitor的遍历函数，中序遍历访问结点node为根结点的子树的结点
 * 参数：
 *      visitor:访问器Visitor
 */
template <class K, class V>
void TreeMap<K,V>::traversal(MapNode<K,V>* node, Visitor<K,V> *visitor)
{
    if(node==nullptr||visitor->stop)
        return;
    traversal(node->left,visitor);
    if(visitor->stop)
        return;
    visitor->visit(node->key,node->value);
    traversal(node->right,visitor);
}

/*
 * 函数功能：带visitor的遍历函数，中序遍历访问整个二叉搜索树的结点
 * 参数：node:树的结点
 *      visitor:访问器Visitor
 */
template <class K, class V>
void TreeMap<K,V>::traversal(Visitor<K,V> *visitor)
{
    if(visitor==nullptr)
        return;
    traversal(mRoot,visitor);
}


/*
 * 函数功能：给定结点x和键值key，在x的子树中寻找key对应的value
 * 参数：x 树的结点
 *      key 要查找的键
 */
template <class K, class V>
MapNode<K,V>* TreeMap<K,V>::get(MapNode<K,V>* x, K key) const
{
    if (x== nullptr)
    {
        //cout<<"找不到KEY:"<<key<<"对应的VALUE"<<endl;
        return x;
    }

    if(x->key==key)
        return x;
    if (key < x->key)
        return get(x->left, key);
    else
        return get(x->right, key);
}

/*
 * 函数功能：给定键值key，在根结点的子树中，也就是整棵树中寻找key对应的value。
 * 参数：key，给定的键值
 */
template <class K, class V>
V TreeMap<K,V>::get(K key)
{
    MapNode<K,V>* mp=get(mRoot, key);
    if(mp!=NULL)//空值判断
        return mp->value;
    else
        return NULL;
}


/*
 * 函数功能：给定结点x，找结点x的前驱结点。即查找"二叉树中key值小于该结点"的"最大结点"。
 * 参数：x:树的某结点
 */
template <class K, class V>
MapNode<K,V>* TreeMap<K,V>::predecessor(MapNode<K,V> *x)
{
    // 如果x存在左孩子，则"x的前驱结点"为 "以其左孩子为根的子树的最大结点"。
    if (x->left != NULL)
        return maximum(x->left);

    // 如果x没有左孩子。则x有以下两种可能：
    // (1) x是"一个右孩子"，则"x的前驱结点"为 "它的父结点"。
    // (2) x是"一个左孩子"，则查找"x的最低的父结点，并且该父结点要具有右孩子"，找到的这个"最低的父结点"就是"x的前驱结点"。
    MapNode<K,V>* y = x->parent;
    while ((y!=NULL) && (x==y->left))
    {
        x = y;
        y = y->parent;
    }

    return y;
}

/*
 * 函数功能：给定结点x，找结点x的后继结点。即，查找"二叉树中key值大于该结点"的"最小结点"。
 * 参数：x:树的某结点
 */
template <class K, class V>
MapNode<K,V>* TreeMap<K,V>::successor(MapNode<K,V> *x)
{
    // 如果x存在右孩子，则"x的后继结点"为 "以其右孩子为根的子树的最小结点"。
    if (x->right != NULL)
        return minimum(x->right);

    // 如果x没有右孩子。则x有以下两种可能：
    // (1) x是"一个左孩子"，则"x的后继结点"为 "它的父结点"。
    // (2) x是"一个右孩子"，则查找"x的最低的父结点，并且该父结点要具有左孩子"，找到的这个"最低的父结点"就是"x的后继结点"。
    MapNode<K,V>* y = x->parent;
    while ((y!=NULL) && (x==y->right))
    {
        x = y;
        y = y->parent;
    }
    return y;
}



/*
 * 函数功能：插入结点z，并返回被插入的结点对应的value值。
 * 参数: tree 二叉树的根结点
 *      z 插入的结点
 * 注意：如果被插入的结点的key已经存在，则返回的value是被覆盖的value。
 */
template <class K, class V>
V TreeMap<K,V>::add(MapNode<K,V>* &tree, MapNode<K,V>* z)
{
    MapNode<K,V> *y = NULL;
    MapNode<K,V> *x = tree;
    // 查找z的插入位置
    while (x != NULL)
    {
        y = x;
        if(z->key==x->key)
        {
            V temp=x->value;
            x->value=z->value;
            return temp;
        }
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
        return z->value;
    }
    else if (z->key < y->key)
    {
        y->left = z;
        len+=1;
        return z->value;
    }
    else
    {
        y->right = z;
        len+=1;
        return z->value;
    }
}

/*
 * 函数功能：给定键值对，往整个二叉搜索树中插入键值对构成的结点。
 * 参数: key:给定的键
 *      value:给定的值
 */
template <class K, class V>
V TreeMap<K,V>::add(K key,V value)
{
    MapNode<K,V> *z=NULL;
    V v=NULL;
    // 如果新建结点失败，则返回空值。
    if ((z=new MapNode<K,V>(key,value,NULL,NULL)) == NULL)
        return v;
    v=add(mRoot, z);
    return v;
}



/*
 * 函数功能：删除结点z，并返回被删除的结点。
 * 参数: tree 二叉树的根结点
 *      z 删除的结点
 */
template <class K, class V>
MapNode<K,V>* TreeMap<K,V>::remove(MapNode<K,V>* &tree, MapNode<K,V> *z)
{
    MapNode<K,V> *x=NULL;
    MapNode<K,V> *y=NULL;

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
template <class K, class V>
V TreeMap<K,V>::remove(K key)
{
    MapNode<K,V> *z, *node;
    if ((z = get(mRoot, key)) != NULL)
    {
        if ( (node = remove(mRoot, z)) != NULL) {
            V v0=node->value;
            delete node;
            cout<<"\n删除键为"<<key<<"的结点成功！"<<endl;
            len--;
            return v0;
        }
    }
    else
    {
        cout<<"\n键为"<<key<<"的结点不在Map中，删除无效！"<<endl;
        return NULL;
    }

}

/*
 * 递归销毁map
 */
//template <class K, class V>
//void TreeMap<K,V>::destroy(MapNode<K,V>* &tree)
//{
//    if (tree==NULL)
//        return ;
//
//    if (tree->left != NULL)
//        return destroy(tree->left);
//    if (tree->right != NULL)
//        return destroy(tree->right);
//
//    delete tree;
//    tree=NULL;
//}

//template <class K, class V>
//void TreeMap<K,V>::clear()
//{
//    destroy(mRoot);
//}


/*
 * 函数功能：寻找以tree为根结点的子树中的最大值
 * 参数：tree:树的结点
 */
template <class K, class V>
MapNode<K,V>* TreeMap<K,V>::maximum(MapNode<K,V>* tree)
{
    if (tree == NULL)
        return NULL;

    while(tree->right != NULL)
        tree = tree->right;
    return tree;
}

/*
 * 函数功能：寻找以tree为根结点的子树中的最小值
 * 参数：tree:树的结点
 */
template <class K, class V>
MapNode<K,V>* TreeMap<K,V>::minimum(MapNode<K,V>* tree)
{
    if (tree == NULL)
        return NULL;
    while(tree->left != NULL)
        tree = tree->left;
    return tree;
}


#endif //CL1_MAP_H
