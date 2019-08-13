## binary pattern search
#### description:
> base on sunday algorithm and support
> wildcard

#### dependence:
> dependence boost1.6

#### example:
```c++
// easy right?
std::vector<std::uintptr_t> out;
Search(std::string("fa ff 00 ?? ff"),0x7fffffff,0x2000,out);
```