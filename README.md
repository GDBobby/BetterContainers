This repo is on the backburner, I'm working on it only when required by other projects. BBC is short for Bobby's Better Containers.

### Very Early notes

1. I think making move and copy instructors and assignment operators implicit was a mistake.

   1.1 At the very least, there should be a compiler flag to turn them off by default, outside of fundamental types.
   
   1.2 I could be convinced implicit copy/move isn't the issue, but then I think I would still say Vector is largely OVERUSED.

2. Vector is too generic. It also doesn't directly support in place construction, requiring the underlying type to be trivially constructible, or movable/copyable.
3. Vector could be broken up into multiple containers.
   
   3.1 RuntimeArray allows construction in place for any size, but doesn't allow resizing.
   
   3.2 DynamicArray is similar to RuntimeArray, but allows resizing.
   
   3.3 KeyValueContainer is a bit of mix between unordered_map and Vector. (KeyValueContainer is a very poor implementation at the moment, my current use cases are about 100 Key Value Pairs or less)

4. StackBlock is basically std::array, but again, allows construction in place.
5. PerFlight is specifically for Graphics applications that, paraphrased, need to have a separate Read and Write buffer.
6. Until Hive comes to the C++ std, I have my own implementation. Probably also not greatly optimized right now. If std::hive doesn't allow construction in place, I'll most likely be working on this further.

To expand on why I don't like vector so much, C++ can be an extremely explicit language. With concepts and templates, the program can be made to do EXACTLY what you want, nothing more, and nothing less.
For example,

`
float sqrt(float);
double sqrt(double);
`

you wouldnt see an int implementation. Now, answer this, which overload is the compiler going to pick if you pass an int32? uint32? int64? boolean? all of these are implicitly castable.
some of you freaks are going to crack open the standard and hit me with some "well the standard chapter 36, section 48ba2 specifies that the int should be casted to a double in this case" or something.
The end user should not need external resources to intuit what your program is doing. they shouldn't need to open documentation, or read comments, or phone a friend.

`template<std::floating_point F> F sqrt(F);`

solves everything. until it doesn't. your user wants to use a 16 bit or 128 bit float. but the case is rare enough that you don't see it fit to implement a specialization for it. back again to vague hidden behavior, convert half to float or double?

`template<std::floating_point F> requires(std::is_same_v<F, float> || std::is_same_v<F, double>) F sqrt(F);`

now the user is forced to decide how they want it casted, and it's no longer even somewhat ambiguous.

The problem comes in full force with vector. Is my data going to be moved or copied when it resizes? what if I want to append a vector? copied or moved? Extremely vague, and again, without cracking open the standard or doing some deep debug inspection, you can't tell. Sure, you could argue an experienced C++ programmer should know which happens when, and that libraries shouldn't be designed for inexperienced programmers (or bad vets), but there's more to it. It's gonna be common that you only initialize your data once, and then never move or copy it. Just like enums with underlying types, you shouldnt allow an enum to be casted to the underlying type unless it's going to be used as the underlying type, using a container with move/copy functionality should imply you're going to use move/copy. 

So, vector should be broken up and split into multiple separate containers. 
