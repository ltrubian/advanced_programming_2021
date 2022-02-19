#include <vector>
#include <iostream>
template <typename T, typename N = std::size_t>
class stack_pool;

template <typename T, typename N = std::size_t>
class Iter : stack_pool<T,N>{
  private:
  N m_ptr;
  stack_pool<T,N>* pool;

  public: 
  using iterator_category = std::forward_iterator_tag;
  using difference_type   = std::ptrdiff_t;
  using value_type        = N;
  using pointer           = N*; 
  using reference         = N&; 

  explicit Iter(N ptr, stack_pool<T,N>* _pool) : m_ptr{ptr}, pool{_pool} {}

  T& operator*() const { return pool->value(m_ptr); }

  Iter operator++() { 
    m_ptr = pool->next(m_ptr);
    return *this;
  }  

  Iter operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  friend bool operator == (const Iter& a, const Iter& b) { return a.m_ptr == b.m_ptr; };
  friend bool operator != (const Iter& a, const Iter& b) { return a.m_ptr != b.m_ptr; }; 

};


template <typename T, typename N >
class stack_pool{
  struct node_t{
    T value;
    N next;
  };
  std::vector<node_t> pool;
  using stack_type = N;
  using value_type = T;
  using size_type = typename std::vector<node_t>::size_type;
  stack_type free_nodes; // at the beginning, it is empty

  node_t& node(stack_type x) noexcept { return pool[x-1]; }
  const node_t& node(stack_type x) const noexcept { return pool[x-1]; }
  
  public:
  stack_pool() : pool{}, free_nodes{stack_type(0)} {} ; // pool{} is not required
  explicit stack_pool(size_type n) : stack_pool{} { pool.reserve(n); }; // reserve n nodes in the pool, delegating constructor
  
  using iterator = Iter<T,N>;
  using const_iterator = Iter<const T,N>;

  iterator begin(stack_type x)  {return iterator{x, this}; } ;
  iterator end(stack_type )  { return iterator{stack_type(0), this}; }; // this is not a typo
    
  const_iterator begin(stack_type x) const  {return iterator{x, this};};
  const_iterator end(stack_type ) const  { return iterator{stack_type(0), this}; }; 
  
  const_iterator cbegin(stack_type x) const  {return iterator{x, this};};
  const_iterator cend(stack_type ) const  { return iterator{stack_type(0), this}; };
    
  stack_type new_stack() const noexcept {return stack_type(0);}; // return an empty stack

  void reserve(const size_type n) { // reserve n nodes in the pool
    std::vector<node_t> tmp(n);
    for ( std::size_t i {0}; i < pool.size() ; ++i)
      tmp[i] = std::move(pool[i]);
    pool = std::move(tmp);
  }; 
  size_type capacity() const noexcept{ return pool.capacity(); }; // the capacity of the pool

  bool empty(const stack_type x) const noexcept {return ( x > 0 ) ? false : true ;} ;

  stack_type end() const noexcept { return stack_type(0); }

  T& value(const stack_type x) noexcept { return node(x).value; };
  const T& value(const stack_type x) const noexcept { return node(x).value; };

  stack_type& next(const stack_type x) noexcept { return node(x).next; };
  const stack_type& next(const stack_type x) const noexcept{ return node(x).next; };

  template <typename X>
  stack_type _push(X&& val, stack_type head) {
    if (static_cast<int>(capacity()) - static_cast<int>(free_nodes) <= 0 ){
      reserve(capacity() + stack_type(1) + capacity()/2 ) ;
    }

    stack_type new_head = empty(free_nodes)? stack_type(1) : free_nodes;
    
    if ( empty(free_nodes) || empty(next(free_nodes)) ){
      free_nodes = stack_type(new_head + 1);
      next(free_nodes) = end();
    }
    else      
      free_nodes = next(free_nodes);
    
    value(new_head) = std::forward<X>(val);
    next(new_head) = ( empty(head) ) ? end() : head;

    return new_head;
  };

  stack_type push(const T& val, stack_type head) { return _push(val, head); } ;

  stack_type push(T&& val, stack_type head){ return _push(std::move(val), head);  };

  stack_type pop (stack_type x) noexcept {
    auto tmp{node(x).next};
    node(x).next = free_nodes;
    free_nodes = x ;
    return tmp;
  }; // delete first node

  stack_type free_stack (stack_type x) noexcept {
    stack_type y{x};
    
    while ( node(y).next != end() )
      y = node(y).next;
    
    node(y).next = free_nodes;
    free_nodes = x ;
    return end();
  }; // free entire stack

};

