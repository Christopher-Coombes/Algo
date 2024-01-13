#include "../General/utils.h"

namespace ds {
	template<class T>
	class linked_list {
	private:
		// Internal node struct
		struct node {
			typedef std::unique_ptr<node> node_ptr;
			node_ptr next;
			T val;

			node(T val);
			node(T val, node_ptr next);
		};
		typedef std::unique_ptr<node> node_ptr;

	public:
		// Iterator
		// https://stackoverflow.com/questions/37031805/preparation-for-stditerator-being-deprecated/38103394
		class iterator {
		public:
			node* tgt;

			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using pointer = T*;
			using reference = T&;
			using iterator_category = std::forward_iterator_tag;

			iterator() : tgt(nullptr) {}
			iterator(node* tgt) : tgt(tgt) {}
			iterator& operator++() { if (tgt) tgt = tgt->next.get(); return *this; }
			iterator operator++(int) { iterator prev = *this; if (tgt) tgt = tgt->next.get(); return prev; }
			bool operator==(iterator that) const { return tgt == that.tgt; }
			bool operator!=(iterator that) const { return tgt != that.tgt; }
			T operator*() { return tgt->val; } // Errors with nullptr access for dereferencing end ptr
			T* operator->() { return &tgt->val; }
		};

		iterator begin() const;
		iterator end() const;

	private:
		// Internal fields
		node_ptr head;

	public:
		// External methods
		linked_list();
		linked_list(std::initializer_list<T> il);

		int length() const;
		void push(T elem);
		T pop();
		T at(int i) const;
		T remove(iterator i);
		T remove_at(int i);
		void insert_after(iterator i, T elem);
		void insert_at(int i, T elem);
	};
}

namespace tester {
	namespace linked_list {
		template<class T> // T has to be writable to ostream
		std::string to_string(ds::linked_list<T>& ll);

		void all(std::ostream& stream);
	};
}