# TinySTL
2025.3.21  
This is my first project of c++STL and also it is my c++ study notes. Comments and suggestions are most welcome!

## Directory

- [配置器](#适配器)
  - [allocator]()
    - [allocate]()
    - [deallocate]()
  - [constructor]()
    - [construct]()
    - [destroy]()
- [迭代器](a)
  - [iterator]()
## 适配器  
allocator 负责**空间的配置与回收**  
constructor 负责**对象的构造与析构**  

## 迭代器
连接着容器与算法，是一种泛型指针（generic pointer，可以指向任何数据类型的指针），每个容器都附带专属的迭代器，是一种重载了*,->，++，--等操作的模板类