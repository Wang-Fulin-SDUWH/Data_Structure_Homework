//
// @author 201800820139 18数据科学班 王福临
// Created by Wanghaha23333 on 2021/8/8.
//

#ifndef CL1_TREESET_H
#define CL1_TREESET_H
#include "BST.h"
#include <iostream>


template <class T>
class TreeSet{
private:
    int len=0;
    BSTree<T> *mytree = new BSTree<T>();
public:
    //所需要的接口直接全部继承BST.h中二叉搜索树的实现
    int size(){
        return mytree->size();
    }
    bool isEmpty(){
        return mytree->isEmpty();
    }
    void clear(){
        mytree->clear();
    }
    bool contains(T key){
        return mytree->contains(key);
    }
    void add(T key){
        //这里的add2指的是BST中实现的去重添加，BST.h文件中的add是非去重的添加。
        return mytree->add2(key);
    }
    void remove(T key){
        mytree->remove(key);
    }
    void traversal(Visitor<T> *visitor){
        mytree->traversal(visitor);
    }
};

#endif //CL1_TREESET_H
