#include "linked_list.h"

namespace ds {
	// Node struct
	template<class T> linked_list<T>::node::node(T val) : val(val), next(nullptr) {}
	template<class T> linked_list<T>::node::node(T val, node_ptr next) : val(val), next(std::move(next)) {}

	// Constructors
	template<class T> linked_list<T>::linked_list() : head(nullptr) {}
	template<class T> linked_list<T>::linked_list(std::initializer_list<T> il) {
		node* tail = nullptr;
		for (const T elem : il) {
			if (tail) {
				tail->next = std::make_unique<node>(elem, nullptr);
				tail = tail->next.get();
			} else {
				head = std::make_unique<node>(elem, nullptr);
				tail = head.get();
			}
		}
	}
	
	// Other methods
	template<class T> int linked_list<T>::length() const {
		node* n = head.get();
		int i = 0;
		while (n) {
			i++;
			n = n->next.get();
		}
		return i;
	}
	template<class T> void linked_list<T>::push(T elem) {
		head = std::make_unique<node>(elem, std::move(head));
	}
	template<class T> T linked_list<T>::pop() {
		if (head) {
			T temp = head->val;
			head = std::move(head->next);
			return temp;
		}
		throw std::out_of_range("Pop from empty linked list");
	}
	template<class T> T linked_list<T>::at(int i) const {
		if (i >= 0) {
			node* n = head.get();
			while (i && n) {
				i--;
				n = n->next.get();
			}
			if (n) return n->val;
		}
		throw std::out_of_range("Linked list index out of range");
	}
	template<class T> T linked_list<T>::remove(iterator i) {
		iterator end = this->end();
		if (i != end) {
			T temp = *i;
			if (i == begin()) {
				head = std::move(i.tgt->next);
				return temp;
			}
			iterator p, it = begin();
			while (it != end) {
				p = it++;
				if (i == it) {
					p.tgt->next = std::move(i.tgt->next);
					return temp;
				}
			}
			throw std::logic_error("Bad iterator?");
		}
		throw std::out_of_range("Linked list iterator out of range");
	}
	template<class T> T linked_list<T>::remove_at(int i) {
		if (i >= 0 && head) {
			if (i == 0) {
				T temp = head->val;
				head = std::move(head->next);
				return temp;
			}
			i--;
			node* n = head.get();
			while (i && n) {
				i--;
				n = n->next.get();
			}
			if (n && n->next) {
				T temp = n->next->val;
				n->next = std::move(n->next->next);
				return temp;
			}
		}
		throw std::out_of_range("Linked list index out of range");
	}
	template<class T> void linked_list<T>::insert_after(iterator i, T elem) {
		i.tgt->next = std::make_unique<node>(elem, std::move(i.tgt->next));
	}
	template<class T> void linked_list<T>::insert_at(int i, T elem) {
		if (i >= 0 && head) {
			if (i == 0) {
				push(elem);
				return;
			}
			i--;
			node* n = head.get();
			while (i && n) {
				i--;
				n = n->next.get();
			}
			if (n) {
				n->next = std::make_unique<node>(elem, std::move(n->next));
				return;
			}
		}
		throw std::out_of_range("Linked list index out of range");
	}

	// Iterator methods
	template<class T> typename linked_list<T>::iterator linked_list<T>::begin() const {
		return iterator(head.get());
	}
	template<class T> typename linked_list<T>::iterator linked_list<T>::end() const {
		return iterator(nullptr);
	}
}

namespace tester {
	namespace linked_list {
		template<class T> // T has to be writable to ostream
		std::string to_string(ds::linked_list<T>& ll) {
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
			ds::linked_list<int> ll{ 1, 2, 3, 4, 5 };
			stream << "Base: " << to_string(ll) << '\n';

			stream << "Pop: " << ll.pop() << " // ";
			stream << to_string(ll) << '\n';

			stream << "Push 6: ";
			ll.push(6);
			stream << to_string(ll) << '\n';

			stream << "Push 7: ";
			ll.push(7);
			stream << to_string(ll) << '\n';

			stream << "Pop: " << ll.pop() << " // ";
			stream << to_string(ll) << '\n';

			stream << "Length: " << ll.length() << '\n';

			stream << "At index 3: " << ll.at(3) << '\n';

			stream << "Remove index 2: " << ll.remove_at(2) << " // ";
			stream << to_string(ll) << '\n';

			auto it = ll.begin(); it++; it++;
			stream << "Remove begin + 2: " << ll.remove(it) << " // ";
			stream << to_string(ll) << '\n';

			it = ll.begin(); it++; it++;
			stream << "Insert 9 after begin + 2: ";
			ll.insert_after(it, 9);
			stream << to_string(ll) << '\n';

			it = ll.begin(); it++; it++; it++;
			stream << "Insert 10 after begin + 3: ";
			ll.insert_after(it, 10);
			stream << to_string(ll) << '\n';

			stream << "Insert 11 at index 5: ";
			ll.insert_at(5, 11);
			stream << to_string(ll) << '\n';

			stream << "Insert 12 at index 0: ";
			ll.insert_at(0, 12);
			stream << to_string(ll) << '\n';
		}
	};
}