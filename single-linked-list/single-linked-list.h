#pragma once
#include <cstddef>
#include <string>
#include <utility>
#include <cassert>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node()
        : next_node(nullptr)
        {
        }
        
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) 
        : node_(node)
        {
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept 
        : node_(other.node_)
        {
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            BasicIterator temp(*this);
            ++(*this);
            return temp;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);
            return &(node_->value);
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{&head_};
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator{&head_};
    }

    SingleLinkedList() = default;

    SingleLinkedList(std::initializer_list<Type> values) {
        for (const auto& value : values) {
            CopyElements(value);
            size_++;
        }
    }

    SingleLinkedList(const SingleLinkedList& other) {
        assert(size_ == 0 && head_.next_node == nullptr);

        Node* other_node = other.head_.next_node;
        while (other_node != nullptr) {
            CopyElements(other_node->value);
            other_node = other_node->next_node;
            size_++;
        }
    }
    
    ~SingleLinkedList(){
        Clear();
    }

    template <typename T>
    void CopyElements(const T& node){
        Node* new_node = new Node(node, nullptr);
        if (head_.next_node == nullptr) {
            head_.next_node = new_node;
        } else {
            Node* last_node = head_.next_node;
            while (last_node->next_node != nullptr) {
                last_node = last_node->next_node;
            }
            last_node->next_node = new_node;
        }
    }
    
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0 ? true : false;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        Node* newNode = new Node{value, pos.node_->next_node};
        Node* current = pos.node_;
        try
        {
            newNode->next_node = current->next_node;
            current->next_node = newNode;
            ++size_;
            return Iterator(newNode);
        }
        catch(...)
        {
            delete newNode;
            throw;
        }
    }

    void PopFront() noexcept {
        if (head_.next_node != nullptr) {
            Node* nodeToDelete = head_.next_node;
            head_.next_node = nodeToDelete->next_node;
            delete nodeToDelete;
            --size_;
        }
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(pos.node_ != nullptr);
        Node* current = pos.node_->next_node;
        pos.node_->next_node = current->next_node;
        delete current;
        --size_;
        return Iterator(pos.node_->next_node);
    }

    void PushFront(const Type& value) {
        Node* newNode = new Node(value, head_.next_node);
        try {
            head_.next_node = newNode;
            newNode = nullptr;
            ++size_;
        } catch (...) {
            delete newNode;
            --size_;
            throw;  
        }
    }

    void Clear() {
        Node* currentNode = head_.next_node;
        while (currentNode != nullptr) {
            Node* nextNode = currentNode->next_node;
            delete currentNode;
            currentNode = nextNode;
        }
        head_.next_node = nullptr;
        size_ = 0;
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if(this != &rhs){
            auto lhs(rhs);
            swap(lhs);
        }
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        Node* new_node = head_.next_node;
        int new_size = size_;
        head_.next_node = other.head_.next_node;
        size_ = other.size_;
        other.head_.next_node = new_node;
        other.size_ = new_size;
    }
    
private:
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if(lhs.GetSize() != rhs.GetSize()){
        return false;
    }
    if(std::equal(lhs.begin(), lhs.end(), rhs.begin())){
        return true;
    }
    return false;
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if(std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())){
        return true;
    }
    return false;
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}
