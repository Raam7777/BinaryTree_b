
#include <iostream>
#include <stdexcept>
#include <queue>

using namespace std;

namespace ariel
{
    template <class T>
    class BinaryTree
    {
        struct Node
        {
            T data;
            Node *left = nullptr;
            Node *right = nullptr;

            Node(const T &d) : data(d) {}
        };

        class iteratorClass
        {
            private:
                queue<Node*> it; // iterator
                Node* node;
            public:
                iteratorClass(Node *ptr = nullptr , int order=1): node(ptr){
                    if (ptr == nullptr){
                        return;
                    }
                    switch(order) {
                        case 1: preOrder(node, it);
                            break;
                        case 2: inOrder(node, it);
                            break;
                        case 3: postOrder(node, it);
                            break;
                    }
                    it.push(nullptr);
                    node = it.front();
                    it.pop();
                }
                T &operator*() const {
                    return node->data;
                }
                T *operator->() const {
                    return &(node->data);
                }
                void inOrder(Node* root, queue<Node*>& itr) {
                    if (root == nullptr){
                        return;
                    }
                    inOrder(root->left, itr);
                    itr.push(root);
                    inOrder(root->right, itr);
                }
                void preOrder(Node* root, queue<Node*>& itr) {
                    if (root == nullptr){
                        return;
                    }
                    itr.push(root);
                    preOrder(root->left, itr);
                    preOrder(root->right, itr);
                }
                void postOrder(Node* root, queue<Node*>& itr) {
                    if (root == nullptr){
                        return;
                    }
                    postOrder(root->left, itr);
                    postOrder(root->right, itr);
                    itr.push(root);
                }
                bool operator==(const iteratorClass &oth) const {
                    return node == oth.node;
                }
                bool operator!=(const iteratorClass &oth) const {
                    return node != oth.node;
                }
                iteratorClass &operator++() {
                    node = it.front();
                    it.pop();                                                              
                    return *this;
                }
                iteratorClass operator++(int){
                    iteratorClass ans = *this;
                    ++(*this);
                    return ans;
                }
        };

    Node *root;

    public:
        BinaryTree<T>(){
            root = nullptr;
        }

        // copy constructor
        BinaryTree(const BinaryTree &otherTree) {
            if (otherTree.root != nullptr) {
                this->root = new Node(otherTree.root->data);
                fillTree(root, otherTree.root);
            }
        }
        /*
        BinaryTree(BinaryTree &&otherTree)noexcept {
            // *this.root = otherTree.root
            this->root = otherTree.root;
            otherTree.root = nullptr;
        }*/

        //destructor
        ~BinaryTree<T>(){}

        void swap(BinaryTree &otherTree) {
            using std::swap;
            swap(root, otherTree.root);
        }

        BinaryTree& operator=(const BinaryTree &otherTree) {
            BinaryTree(otherTree).swap(*this);
            return *this;
        }
        /*
        BinaryTree& operator=(BinaryTree<T>&& otherTree)noexcept {
            BinaryTree(otherTree).swap(*this);
            return *this;
        }
        */
        BinaryTree<T> &add_root(const T &data){
            if(root == nullptr) {
                root = new Node(data);
            }
            else {
                root->data = data;
            }
            return *this;
        }
        BinaryTree<T> &add_left(const T &data, const T &left){
            if(!(ifNodeExist(data))){
                throw runtime_error("Node Not Found!");
            }
            Node* ptrNode = nodeSearch(data, root);
            if(ptrNode != nullptr) {
                if(ptrNode->left == nullptr){
                    ptrNode->left = new Node(left);
                }
                else{
                    ptrNode->left->data = left;
                }
            }
            return *this;
        }
        BinaryTree<T> &add_right(const T &data, const T &right){
            if(!(ifNodeExist(data))){
                throw runtime_error("Node Not Found!");
            }
            Node* ptrNode = nodeSearch(data, root);
            if(ptrNode != nullptr) {
                if(ptrNode->right == nullptr){
                    ptrNode->right = new Node(right);
                }
                else{
                    ptrNode->right->data = right;
                }
            }
            return *this;
        }
        iteratorClass begin_preorder(){
            return iteratorClass(root,1);
        }
        iteratorClass end_preorder(){
            return iteratorClass(nullptr,1);
        }
        iteratorClass begin_inorder(){
            return iteratorClass(root,2);
        }
        iteratorClass end_inorder(){
            return iteratorClass(nullptr,2);
        }
        iteratorClass begin_postorder(){
            return iteratorClass(root,3);
        }
        iteratorClass end_postorder(){
            return iteratorClass(nullptr,3);
        }
        iteratorClass begin(){
            return iteratorClass(root,2);
        }
        iteratorClass end(){
            return iteratorClass(nullptr,2);
        }
        friend std::ostream& operator<<(std::ostream& output,BinaryTree<T> &tree){
            output << "Inorder : \n";
            for (auto i = tree.begin_inorder(); i != tree.end_inorder(); ++i)
            {   
                output << *i << " , " ;
            }
            output << "\n";

            output << "Preorder : \n";
            for (auto i = tree.begin_preorder(); i != tree.end_preorder(); ++i)
            {   
                output << *i << " , " ;
            }
            output << "\n";

            output << "Postorder : \n";
            for (auto i = tree.begin_postorder(); i != tree.end_postorder(); ++i)
            {   
                output << *i << " , " ;
            }
            output << "\n";

            return output;
        }

        void fillTree(Node *root, Node *son) {
            if (son->right != nullptr) {
                root->right = new Node(son->right->data);
                fillTree(root->right, son->right);
            }
            if (son->left != nullptr) {
                root->left = new Node(son->left->data);
                fillTree(root->left, son->left);
            }
        }

        //function to search for a specific node in a tree , return ptr of the node
        Node* nodeSearch(const T& key, Node* searchNode) {
            if (searchNode == NULL){
                return NULL;
            }
            if (searchNode->data == key){
                return searchNode;
            }
        
            auto res1 = nodeSearch(key,searchNode->left);
            // node found, no need to look further
            if(res1 != NULL){
                return res1;
            }
        
            /* node is not found in left,
            so recur on right subtree */
            auto res2 = nodeSearch(key,searchNode->right);
            return res2;
        }

        // Function to traverse the tree in preorder
        // and check if the given node exists in it
        bool nodeExists(struct Node* node, const T &key)
        {
            if (node == NULL){
                return false;
            }
        
            if (node->data == key){
                return true;
            }
        
            /* then recur on left sutree */
            bool res1 = nodeExists(node->left, key);
            // node found, no need to look further
            if(res1){
                return true;
            }
        
            /* node is not found in left,
            so recur on right subtree */
            bool res2 = nodeExists(node->right, key);
        
            return res2;
        }

        // Function to traverse the tree in preorder
        // and check if the given node exists in it
        bool ifNodeExist(const T &key)
        {
            return nodeExists(root,key);
        }
    }; 
} 