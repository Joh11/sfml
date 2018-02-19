#ifndef GC_HPP
#define GC_HPP

namespace gc
{
    template <typename T> class Pointer;
    
    template <typename T>
    class Value
    {
	// So the pointer can update the counter
	friend class Pointer<T>;
	// So only a garbage collector can create and destroy values
	
    private:
	T _value;
	int _counter;
    };

    template <typename T>
    class Pointer
    {
    public:
	Pointer(Value<T> *v) : _ref{v} {++_ref._counter; }
	Pointer(Pointer<T> const& p) : _ref{p._ref} {++_ref._counter; }

	~Pointer() {--_ref._counter; }
    private:
	Value<T>* _ref;
    };
}

#endif
