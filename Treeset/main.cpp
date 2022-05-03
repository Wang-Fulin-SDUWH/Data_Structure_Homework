/**
 * C++ 语言: 二叉查找树
 *
 * @author 201800820139 18数据科学班 王福临
 * @date 2021/08/08
 * 在C++中用纯虚函数和类的继承实现了Visitor，详细实现在文件BST.h中。
 */

#include <iostream>
#include "treeset.h"
using namespace std;

static int arr[]= {1,5,4,3,2,6};
#define TBL_SIZE(a) ( (sizeof(a)) / (sizeof(a[0])) )

int main()
{
    // 实例化一个TreeSet类，命名为tree
    int i, ilen;
    TreeSet<int>* tree=new TreeSet<int>();

    //测试isEmpty接口
    cout<<"树是否为空:"<<tree->isEmpty()<<endl;

    //测试add接口
    cout << "== 依次添加: ";
    ilen = TBL_SIZE(arr);
    for(i=0; i<ilen; i++)
    {
        cout << arr[i] <<" ";
        tree->add(arr[i]);
        tree->add(i+1);//这里刻意重复添加，以展示效果
    }

    //测试size接口
    cout<<"\n实时元素个数："<<tree->size();

    //测试remove接口
    cout << "\n== 删除节点: " << arr[3];
    tree->remove(arr[3]);
    cout << "\n== 删除节点: " << 100;
    tree->remove(100);

    //再次测试size和isEmpty接口
    cout<<"\n实时元素个数："<<tree->size();
    cout<<"\n树是否为空:"<<tree->isEmpty()<<endl;
    cout << endl;

    //测试contains接口
    cout << "\n是否包含3: "<<tree->contains(3);
    cout << "\n是否包含5: "<<tree->contains(5);
    cout << "\n是否包含6: "<<tree->contains(6);
    cout << "\n是否包含7: "<<tree->contains(7)<<endl;

    //测试traversal接口，在这里自定义Visitor中的visit函数
    class Visitor1:public Visitor<int>{
        void visit(int key){
            cout<<"Visitor定制访问Treeset节点"<<key<<endl;
        }
    };
    Visitor1 *visitor=new Visitor1();
    tree->traversal(visitor);

    // 测试clear接口
    tree->clear();

    return 0;
}