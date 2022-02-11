#include<iostream>
template <typename T, typename N = std::size_t>
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
  stack_pool() : free_nodes{0} {} ;
  explicit stack_pool(size_type n) : free_nodes{0} { pool.reserve(n); }; // reserve n nodes in the pool
    
  using iterator = stack_type*;
  using const_iterator = const stack_type*;

  iterator begin(stack_type x) noexcept {return (&x)-1;} ;
  iterator end(stack_type ) noexcept { return stack_type(0); }; // this is not a typo
    
  const_iterator begin(stack_type x) const noexcept {return (&x)-1;};
  const_iterator end(stack_type ) const noexcept { return stack_type(0); }; 
  
  const_iterator cbegin(stack_type x) const noexcept {return (&x)-1;};
  const_iterator cend(stack_type ) const noexcept { return stack_type(0); };
    
  stack_type new_stack() const noexcept {return stack_type(0);}; // return an empty stack

  void reserve(size_type n) {pool.reserve(n); }; // reserve n nodes in the pool
  size_type capacity() const noexcept{ return pool.capacity(); }; // the capacity of the pool

  bool empty(stack_type x) const  {return ( x > 0 ) ? false : true ;} ;

  stack_type end() const noexcept { return stack_type(0); }

  T& value(stack_type x) noexcept { return node(x).value; };
  const T& value(stack_type x) const noexcept { return node(x).value; };

  stack_type& next(stack_type x) noexcept { return node(x).next; };
  const stack_type& next(stack_type x) const noexcept{ return node(x).next; };

  stack_type push(const T& val, stack_type head) {
    if (empty(free_nodes)){
      std::cout << "here\n";
      pool[0].value = val;
      pool[0].next = head - 1 ;
      (free_nodes) += 1;
      pool[free_nodes].next = pool.end();
      return 1;
    }
    else{
      std::cout << "or here??\t" << free_nodes << "\n";
      stack_type new_head{free_nodes};
      pool[new_head].value = val;
      pool[new_head].next = head - 1;
      if ( pool[free_nodes].next == stack_type(0)){
        (free_nodes) += 1;
        // ++(free_nodes);
        pool[free_nodes].next = stack_type(0);        
      }
      else{
        free_nodes = pool[free_nodes].next;
      }
      return ++new_head;
    }

  } ;
  stack_type push(T&& val, stack_type head){
    if (empty(free_nodes)){
      // std::cout << "here\t"<< free_nodes << "\n";
      pool[0].value = val;
      pool[0].next = head - 1 ;
      (free_nodes) += 1;
      // std::cout << "here\t"<< free_nodes << "\n";
      // ++(free_nodes);
      pool[free_nodes].next = stack_type(0);
      return 1;
    }
    else{
      // std::cout << "or here??\t" << free_nodes << "\n";
      stack_type new_head{free_nodes};
      pool[new_head].value = val;
      pool[new_head].next = head - 1;
      if ( pool[free_nodes].next == stack_type(0)){
        (free_nodes) += 1;
        // ++(free_nodes);
        pool[free_nodes].next = stack_type(0);        
      }
      else{
        free_nodes = pool[free_nodes].next;
      }
      return ++new_head;
    }
  };
  stack_type pop(stack_type x){
    node(x).next = free_nodes;
    free_nodes = x - 1;
    return x - 1;
  }; // delete first node

  stack_type free_stack(stack_type x){
    stack_type y{x};
    while ( node(y).next != stack_type{0} )
      ++y;
    pop(y);
    return stack_type{0};
  }; // free entire stack
};