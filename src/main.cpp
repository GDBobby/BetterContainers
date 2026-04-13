#include "../BBC/HeapBlock.h"
#include "../BBC/RuntimeArray.h"

#include <cstdio>

using namespace BBC;
    int main(){
        HeapBlock<int> first{10};
        first.Fill(3);

        HeapBlock<int> second{10};
        for(int i = 0; i < 10; i++){
            second[i] = i;
        }
        printf("first before\n");
        for(int i = 0; i < 10; i++){
            printf("\t[%d]:%d\n", i, first[i]);
        }

        CopyAssign<HeapBlock<int>>::Ex(first, second);

        printf("first after\n");
        for(int i = 0; i < 10; i++){
            printf("\t[%d]:%d\n", i, first[i]);
        }

        RuntimeArray<int> test{5, 4}; //constructs 5 ints, each with a value of 4
        for(auto& val : test){
            printf("val %d\n", val);
        }

        return 0;
    }