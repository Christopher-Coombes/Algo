#include "deque.h"

namespace ds {
	// Node struct
	template<class T> deque<T>::node::node(T val) : val(val), next(nullptr), prev(nullptr) {}
	template<class T> deque<T>::node::node(T val, node_ptr next, node* prev) : val(val), next(std::move(next)), prev(prev) {}
	
	// Constructors
	template<class T> deque<T>::deque() : head(nullptr), tail(nullptr) {}
	template<class T> deque<T>::deque(std::initializer_list<T> il) {
		tail = nullptr;
		for (const T elem : il) {
			if (tail) {
				tail->next = std::make_unique<node>(elem, nullptr, tail);
				tail = tail->next.get();
			} else {
				head = std::make_unique<node>(elem, nullptr, nullptr);
				tail = head.get();
			}
		}
	}

	// Other methods
	/*
	T remove_at(int i);
	T remove_back(int i);
	T remove(iterator i);
	void insert_at(int i, T elem);
	void insert_back(int i, T elem);
	void insert_after(iterator i, T elem);
	void insert_before(iterator i, T elem);*/
	template<class T> int deque<T>::length() const {
		node* n = head.get();
		int i = 0;
		while (n) {
			i++;
			n = n->next.get();
		}
		return i;
	}
	template<class T> void deque<T>::push(T elem) {
		head = std::make_unique<node>(elem, std::move(head), nullptr);
		head->next->prev = head.get();
	}
	template<class T> void deque<T>::push_back(T elem) {
		tail->next = std::make_unique<node>(elem, nullptr, tail);
		tail = tail->next.get();
	}
	template<class T> T deque<T>::pop() {
		if (head) {
			T temp = head->val;
			head = std::move(head->next);
			if (head) head->prev = nullptr;
			return temp;
		}
		throw std::out_of_range("Pop from empty deque");
	}
	template<class T> T deque<T>::pop_back() {
		if (tail) {
			T temp = tail->val;
			if (tail->prev) {
				node* nTemp = tail->prev;
				tail->prev->next.reset();
				tail = nTemp;
			} else {
				head.reset();
				tail = nullptr;
			}
			return temp;
		}
		throw std::out_of_range("Pop from empty deque");
	}
	template<class T> T deque<T>::at(int i) const {
		if (i >= 0) {
			node* n = head.get();
			while (i && n) {
				i--;
				n = n->next.get();
			}
			if (n) return n->val;
		}
		throw std::out_of_range("Deque index out of range");
	}
	template<class T> T deque<T>::at_back(int i) const {
		if (i >= 0) {
			node* n = tail;
			while (i && n) {
				i--;
				n = n->prev;
			}
			if (n) return n->val;
		}
		throw std::out_of_range("Deque index out of range");
	}
	template<class T> T deque<T>::remove(node* n) {
		T temp = n->val;

		if (n->prev && n->next) {
			n->next->prev = n->prev;
			n->prev->next = std::move(n->next);
			return temp;
		}
		if (n->prev) {
			tail = n->prev;
			n->prev->next.reset();
		} else if (n->next) {
			n->next->prev = nullptr;
			head = std::move(n->next);
		}
		return temp;
	}
	template<class T> T deque<T>::remove(iterator i) {
		return remove(i.tgt);
	}
	template<class T> T deque<T>::remove_at(int i) {
		if (i >= 0) {
			node* n = head.get();
			while (i && n) {
				i--;
				n = n->next.get();
			}
			if (n) return remove(n);
		}
		throw std::out_of_range("Deque index out of range");
	}
	template<class T> T deque<T>::remove_back(int i) {
		if (i >= 0) {
			node* n = tail;
			while (i && n) {
				i--;
				n = n->prev;
			}
			if (n) return remove(n);
		}
		throw std::out_of_range("Deque index out of range");
	}
	template<class T> void deque<T>::insert_after(node* n, T elem) {
		if (n) {
			if (n->next) {
				n->next = std::make_unique<node>(elem, std::move(n->next), n);
				n->next->next->prev = n->next.get();
			} else {
				n->next = std::make_unique<node>(elem, nullptr, n);
				tail = n->next.get();
			}
			return;
		}
		throw std::out_of_range("Deque insert beyond end");
	}
	template<class T> void deque<T>::insert_after(iterator i, T elem) {
		insert_after(i.tgt, elem);
	}
	template<class T> void deque<T>::insert_before(node* n, T elem) {
		if (n) {
			if (n->prev) {
				node* prev = n->prev;
				n->prev->next = std::make_unique<node>(elem, std::move(n->prev->next), prev);
				n->prev = prev->next.get();
			} else {
				head = std::make_unique<node>(elem, std::move(head), nullptr);
				head->next->prev = head.get();
			}
		} else {
			if (tail) {
				tail->next = std::make_unique<node>(elem, nullptr, tail);
				tail = tail->next.get();
			} else {
				head = std::make_unique<node>(elem, nullptr, nullptr);
				tail = head.get();
			}
		}
	}
	template<class T> void deque<T>::insert_before(iterator i, T elem) {
		insert_before(i.tgt, elem);
	}

	// Iterator methods
	template<class T> typename deque<T>::iterator deque<T>::begin() const {
		const deque<T>* dis = this;
		return iterator(head.get(), dis);
	}
	template<class T> typename deque<T>::iterator deque<T>::end() const {
		const deque<T>* dis = this;
		return iterator(nullptr, dis);
	}
	template<class T> void deque<T>::insert_at(int i, T elem) {
		if (i >= 0) {
			node* n = head.get();
			while (i) {
				if (!n) throw std::out_of_range("Deque index out of range");
				i--;
				n = n->next.get();
			}
			insert_before(n, elem);
			return;
		}
		throw std::out_of_range("Deque index out of range");
	}
	template<class T> void deque<T>::insert_back(int i, T elem) {
		if (i == 0) {
			insert_before(nullptr, elem);
			return;
		} else if (i >= 0) {
			i--;
			node* n = tail;
			while (i && n) {
				i--;
				n = n->prev;
			}
			if (n) {
				insert_before(n, elem);
				return;
			}
		}
		throw std::out_of_range("Deque index out of range");
	}
}

namespace tester {
	namespace deque {
		template<class T> // T has to be writable to ostream
		std::string to_string(ds::deque<T>& ll) {
			std::stringstream str;
			str << '[';
			auto beg = ll.begin();
			auto end = ll.end();
			if (beg != end) {
				while (true) {
					str << *beg;
					beg++;
					if (beg == end) break;
					str << ", ";
				}
			}
			str << ']';
			return str.str();
		}

		void all(std::ostream& stream) {
			ds::deque<int> dq{ 1, 2, 3, 4, 5 };
			stream << "Base: " << to_string(dq) << '\n';

			stream << "Pop: " << dq.pop() << " // ";
			stream << to_string(dq) << '\n';

			stream << "Push 6: ";
			dq.push(6);
			stream << to_string(dq) << '\n';

			stream << "Push 7: ";
			dq.push(7);
			stream << to_string(dq) << '\n';

			stream << "Pop: " << dq.pop() << " // ";
			stream << to_string(dq) << '\n';

			stream << "Length: " << dq.length() << '\n';

			stream << "Pop Back: " << dq.pop_back() << " // ";
			stream << to_string(dq) << '\n';

			stream << "Push Back 6: ";
			dq.push_back(6);
			stream << to_string(dq) << '\n';

			stream << "Push Back 7: ";
			dq.push_back(7);
			stream << to_string(dq) << '\n';

			stream << "At 0, 2, 5: ";
			stream << dq.at(0) << ", " << dq.at(2) << ", " << dq.at(5) << '\n';

			stream << "At-back 0, 2, 5: ";
			stream << dq.at_back(0) << ", " << dq.at_back(2) << ", " << dq.at_back(5) << '\n';

			stream << "Pop Back: " << dq.pop_back() << " // ";
			stream << to_string(dq) << '\n';

			stream << "Length: " << dq.length() << '\n';

			stream << "Push 10, 11, 12: ";
			dq.push(10); dq.push(11); dq.push(12);
			stream << to_string(dq) << '\n';

			auto it = dq.begin(); it++; it++;
			stream << "Remove begin + 2: ";
			stream << dq.remove(it) << " // ";
			stream << to_string(dq) << '\n';

			it = dq.end();
			it--;
			stream << "Remove end - 1: ";
			stream << dq.remove(it) << " // ";
			stream << to_string(dq) << '\n';

			it = dq.begin();
			stream << "Remove begin: ";
			stream << dq.remove(it) << " // ";
			stream << to_string(dq) << '\n';

			stream << "Iterate in reverse: ";
			it = dq.end();
			while (it != dq.begin()) {
				it--;
				stream << *it << ", ";
			}
			stream << '\n';

			it = dq.begin();
			stream << "Insert 1 after begin: ";
			dq.insert_after(it, 1);
			stream << to_string(dq) << '\n';
			
			stream << "Insert 1 before begin: ";
			dq.insert_before(it, 1);
			stream << to_string(dq) << '\n';

			it = dq.end();
			stream << "Insert 2 before end: ";
			dq.insert_before(it, 2);
			stream << to_string(dq) << '\n';

			it = dq.end(); it--; it--; it--;
			stream << "Insert 2 after end - 3: ";
			dq.insert_after(it, 2);
			stream << to_string(dq) << '\n';
		}
	}
}