#pragma once


namespace BBC{

    template<typename T>
    struct CopyConstruct{
        static T& Ex(T* construction_addr, T const& copySrc);
    };
    template<typename T>
    struct CopyAssign{
        static T& Ex(T& copyDst, T const& copySrc);
    };
    template<typename T>
    struct MoveConstruct{
        static T& Ex(T* construction_addr, T&& moveSrc);
    };
    template<typename T>
    struct MoveAssign{
        static T& Ex(T& moveDst, T&& moveSrc);
    };

}