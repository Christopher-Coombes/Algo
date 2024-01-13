#include "../General/utils.h"

namespace ds {
	template<class T>
	class deque {
	private:
		// Internal node struct
		struct node {
			typedef std::unique_ptr<node> node_ptr;
			node_ptr next; // Ownership goes forward
			node* prev;
			T val;

			node(T val);
			node(T val, node_ptr next, node* prev);
		};
		typedef std::unique_ptr<node> node_ptr;

	public:
		// Iterator
		class iterator {
		public:
			node* tgt;
			const deque<T>* dq;

			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using pointer = T*;
			using reference = T&;
			using iterator_category = std::forward_iterator_tag;

			iterator() : tgt(nullptr), dq(nullptr) {}
			iterator(node* tgt, const deque<T>* dq) : tgt(tgt), dq(dq) {}
			iterator& operator++() {
				if (tgt) tgt = tgt->next.get();
				return *this;
			}
			iterator& operator--() {
				if (tgt) tgt = tgt->prev;
				else if (dq) tgt = dq->tail;
				return *this;
			}
			iterator operator++(int) {
				iterator prev = *this;
				if (tgt) tgt = tgt->next.get();
				return prev;
			}
			iterator operator--(int) {
				iterator prev = *this;
				if (tgt) tgt = tgt->prev;
				else if (dq) tgt = dq->tail;
				return prev;
			}
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
		node* tail;
	
	public:
		// External methods
		deque();
		deque(std::initializer_list<T> il);

		int length() const;
		void push(T elem);
		void push_back(T elem);
		T pop();
		T pop_back();
		T at(int i) const;
		T at_back(int i) const;
		T remove_at(int i);
		T remove_back(int i);
		T remove(node* n);
		T remove(iterator i);
		void insert_at(int i, T elem);
		void insert_back(int i, T elem);
		void insert_after(node* i, T elem);
		void insert_before(node* i, T elem);
		void insert_after(iterator i, T elem);
		void insert_before(iterator i, T elem);

	};
}

namespace tester {
	namespace deque {
		template<class T> // T has to be writable to ostream
		std::string to_string(ds::deque<T>& ll);

		void all(std::ostream& stream);
	}
}