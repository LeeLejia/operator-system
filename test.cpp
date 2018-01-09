#include <iostream>
using namespace std;
// 只处理正数
int getR(int a){
    int k = a/1000;
    int r = a%1000;
    if(k==0){
        cout<<a;
        if(r!=0){
            cout<<',';
        }
    } else{
        getR(k);
        cout<<r;
    }
}
int main(){
    int a,b;
    cin>>a>>b;
    a=a+b;
    if(a<0){
        cout<<'-';
        getR(-a);
    }else{
        getR(a);
    }
    return 0;
}