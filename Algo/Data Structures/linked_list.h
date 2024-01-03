#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <initializer_list>
#include <iostream>
#include "../General/utils.h"

namespace ds {
	template<class T>
	class linked_list {
		struct node {
			typedef std::unique_ptr<node> node_ptr;
			node_ptr next;
			T val;

			node(T val);
			node(T val, node_ptr next);
		};
		typedef std::unique_ptr<node> node_ptr;


	public:
		// https://stackoverflow.com/questions/37031805/preparation-for-stditerator-being-deprecated/38103394
		class iterator {
		public:
			node* tgt;

			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using pointer = T*;
			using reference = T&;
			using iterator_category = std::forward_iterator_tag;

			iterator(node* tgt = nullptr) : tgt(tgt) {}
			iterator& operator++() { if (tgt) tgt = tgt->next.get(); return *this; }
			iterator operator++(int) { iterator prev = *this; if (tgt) tgt = tgt->next.get(); return prev; }
			bool operator==(iterator that) const { return tgt == that.tgt; }
			bool operator!=(iterator that) const { return tgt != that.tgt; }
			T operator*() { return tgt->val; } // Errors with nullptr access for dereferencing end ptr
		};

		iterator begin() const;
		iterator end() const;

		node_ptr head;
		linked_list();
		linked_list(std::initializer_list<T> il);

		int length() const;
		void push(T elem);
		T pop();
		T at(int i) const;
		T remove_at(int i);
		T remove_at(iterator i);
		void insert_at(int i, T elem);
		void insert_at(iterator i, T elem);
	};
	/*
	template<long FROM, long TO>
class Range {
public:
    class iterator {
        long num = FROM;
    public:
        iterator(long _num = 0) : num(_num) {}
        iterator& operator++() {num = TO >= FROM ? num + 1: num - 1; return *this;}
        iterator operator++(int) {iterator retval = *this; ++(*this); return retval;}
        bool operator==(iterator other) const {return num == other.num;}
        bool operator!=(iterator other) const {return !(*this == other);}
        long operator*() {return num;}
        // iterator traits
        using difference_type = long;
        using value_type = long;
        using pointer = const long*;
        using reference = const long&;
        using iterator_category = std::forward_iterator_tag;
    };
    iterator begin() {return FROM;}
    iterator end() {return TO >= FROM? TO+1 : TO-1;}
};
	
	*/
}

namespace tester {
	namespace linked_list {
		template<class T> // T has to be writable to ostream
		std::string to_string(ds::linked_list<T>& ll);

		void all(std::ostream& stream);
	};
}