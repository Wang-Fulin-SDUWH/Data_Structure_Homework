// 201800820139 18级数据科学班 王福临 Map的测试代码
// Map的测试代码
#include <iostream>
#include <string>
#include "map.h"

using namespace std;

int main() {
    //实例化一个Map类
    TreeMap<string,int> *tmap=new TreeMap<string,int>();
    string str[]={"car","factory","cat","eye","dog"};

    //测试isEmpty()接口
    cout<<"Map是否为空:"<<tmap->isEmpty()<<endl;

    //往Map中添加一些元素，测试add接口
    for(int i=0;i<5;i++)
    {
        tmap->add(str[i],i);
    }

    //测试size接口
    cout<<"此时元素个数："<<tmap->size()<<endl;

    //测试add接口重复对一个key进行插入的情况和返回值
    string key001="A666";
    int v1=10,v2=5;
    cout<<"初始化一个key叫做A666，值为"<<tmap->add(key001,v1)<<endl;
    cout<<"用新的值"<<v2<<"去覆盖A666，被覆盖的值为"<<tmap->add(key001,v2)<<endl;

    //第二次测试size接口
    cout<<"此时元素个数："<<tmap->size()<<endl;

    //测试get接口（用get测试空接口时，得到的NULL显示为0）
    cout<<"查询A666在上述操作后对应的值:"<<tmap->get(key001)<<endl;

    //测试删除接口删除一个存在的key
    string key002="eye0";
    key001="eye";
    int delval=tmap->remove(key001);
    cout<<"键为"<<key001<<"的结点被删除，对应的Value为："<<delval<<endl;
    //测试删除接口删除一个不存在的key
    int delval1=tmap->remove(key002);
    cout<<endl;

    //第三次测试size接口
    cout<<"此时元素个数："<<tmap->size()<<endl;

    //测试是否包含某个键和某个值的接口
    cout<<"Map中是否包含键为A666的结点:"<<tmap->containsKey("A666")<<endl;
    cout<<"Map中是否包含键为eye的结点:"<<tmap->containsKey("eye")<<endl;
    cout<<endl;
    cout<<"Map中是否包含值为2的结点:"<<tmap->containsValue(2)<<endl;
    cout<<"Map中是否包含值为3的结点:"<<tmap->containsValue(3)<<endl;
    cout<<endl;

    //测试遍历接口，自定义Visitor的遍历访问方式，只需要修改下面的visit函数。
    class MapVisitor:public Visitor<string, int>{
        void visit(string key,int value){
            cout<<"用Visitor遍历访问Map结点，键为"<<key<<"，值为"<<value<<endl;
        }
    };
    MapVisitor *visitor=new MapVisitor();
    tmap->traversal(visitor);

    //测试清空接口
    tmap->clear();

    return 0;
}
