#ifndef ListSimple_H_INCLUDED
#define ListSimple_H_INCLUDED

#include <exception>
#include <string>
#include <fstream>

///Definicion
template <class T>
class ListSimple {
    private:
        class Node {
            private:
            T data;
            Node* next;

            public:
            Node();
            Node(const T&);

            T getData() const;
            Node* getNext() const;

            void setData(const T&);
            void setNext(Node*); 
        };

        Node* anchor;

        bool isValidPos(Node*) const;

        void copyAll(const ListSimple<T>&);

    public:
        typedef Node* Position;

        class Exception : public std::exception{
        private:
            std::string msg;

        public: 
            explicit Exception(const char* message) : msg(message) {}
            explicit Exception(const std::string& message) : msg(message) {}
            virtual ~Exception() throw () {}
            virtual const char* what() const throw () {
                return msg.c_str();
            }

    };

        ListSimple();
        ListSimple(const ListSimple<T>&);

        ~ListSimple();

        bool isEmpty() const;

        void insertData(Node*, const T&);
        void insertSortedData(Node*, const T&);

        void deleteData(Node*);

        Node* getFirstPos() const;
        Node* getLastPos() const;
        Node* getPrevPos(Node*) const;
        Node* getNextPos(Node*) const;

        Node* findData(const T&) const;

        void swapData(Node*, Node*);
        void Sort();

        T retrieve(Node*);

        void readFromDisk(const std::string&);
        void writeToDisk(const std::string&);

        std::string toString() const;

        void deleteAll();

        ListSimple<T>& operator = (const ListSimple<T>&);
};

///Implementacion

//Nodo
template <class T>
ListSimple<T>::Node::Node() : next(nullptr) { }

template <class T>
ListSimple<T>::Node::Node(const T& e) : data(e), next(nullptr) { }

template <class T>
T ListSimple<T>::Node::getData() const{
    return data;
}

template <class T>
typename ListSimple<T>::Node* ListSimple<T>::Node::getNext() const{
    return next;
}

template <class T>
void ListSimple<T>::Node::setData(const T& e){
    data = e;
}

template <class T>
void ListSimple<T>::Node::setNext(Node* p){
    next = p;
}

//ListSimple

template <class T>
void ListSimple<T>::copyAll(const ListSimple<T>& l){
    Node* aux(l.anchor);
    Node* last(nullptr);
    Node* newNode;

    while(aux != nullptr){
        newNode = new Node(aux->getData());

        if(last == nullptr){
            anchor = newNode;
        }
        else{
            last->setNext(newNode);
        }

        last = newNode;

        aux = aux->getNext();
    }
}

template <class T>
bool ListSimple<T>::isValidPos(Node* p) const{
    Node* aux(anchor);

    while(aux != nullptr){
        if(aux == p){
            return true;
        }
    
    aux = aux->getNext();
    }

    return false;
}

template <class T>
ListSimple<T>::ListSimple() : anchor(nullptr) { }

template <class T>
ListSimple<T>::ListSimple(const ListSimple<T>& l) : anchor(nullptr) {
    copyAll(l);
} 

template <class T>
ListSimple<T>::~ListSimple() {
    deleteAll();
}

template <class T>
bool ListSimple<T>::isEmpty() const{
    return anchor == nullptr;
}

template <class T>
void ListSimple<T>::insertData(Node* p, const T& e) {
    if(p != nullptr && !isValidPos(p)){
        throw Exception("Posicion invalida, insertData");
    }

    Node* aux(new Node(e));

    if(aux == nullptr){
        throw Exception("Memoria no disponible, insertData");
    }

    if(p == nullptr) {
        aux->setNext(anchor);
        anchor=aux;
    }
    else{
        aux->setNext(p->getNext());
        p->setNext(aux);
    }
}

template<class T>
void ListSimple<T>::insertSortedData(Node* p, const T& e) {
    if(p != nullptr && !isValidPos(p)){
        throw Exception("Posicion invalida, insertData");
    }

    Node* aux(new Node(e));

    if(aux == nullptr){
        throw Exception("Memoria no disponible, insertData");
    }

    if(anchor == nullptr) { // caso lista vacía
        anchor = aux;
        aux->setNext(nullptr);
    } else if(p == nullptr || p == anchor) { // caso insertar al inicio
        aux->setNext(anchor);
        anchor = aux;
    } else { // caso insertar en posición intermedia o final
        Node* prev = anchor;
        while(prev->getNext() != p && prev->getNext() != nullptr){
            prev = prev->getNext();
        }
        aux->setNext(prev->getNext());
        prev->setNext(aux);
    }

    Sort();

}

template <class T>
void ListSimple<T>::deleteData(Node* p){
    if(!isValidPos(p)){
        throw Exception("Posicion invalida, deleteData");
    }

    if(p == anchor){
        anchor == anchor->getNext();
    }
    else{
        getPrevPos(p)->setNext(p->getNext());
    }

    delete p;
}

template <class T>
typename ListSimple<T>::Node* ListSimple<T>::getFirstPos() const{
    return anchor;
}

template <class T>
typename ListSimple<T>::Node* ListSimple<T>::getLastPos() const{
    if(isEmpty()){
        return nullptr;
    }

    Node* aux(anchor);
    while(aux->getNext() != nullptr){
        aux = aux->getNext();
        }
    return aux;
} 

template <class T>
typename ListSimple<T>::Node* ListSimple<T>::getPrevPos(Node* p) const {
    if(p == anchor){
        return nullptr;
    }

    Node* aux(anchor);

    while(aux != nullptr && aux->getNext() != p){
        aux = aux->getNext();
        }
    return aux;
} 

template <class T>
typename ListSimple<T>::Node* ListSimple<T>::getNextPos(Node* p) const {
    if(!isValidPos(p)){
        return nullptr;
    }

    return p->getNext();
}

template <class T>
typename ListSimple<T>::Node* ListSimple<T>::findData(const T& e) const {
    Node* aux(anchor);

    while(aux != nullptr && aux->getData() != e){
        aux = aux->getNext();
    }

    return aux;
}

template <class T>
void ListSimple<T>::swapData(Node* p, Node* q){
    T temp = p->getData();
    p->setData(q->getData());
    q->setData(temp);
}

template <class T>
void ListSimple<T>::Sort(){
    Node* p = anchor;
    while (p != nullptr) {
        Node* min = p;
        Node* q = p->getNext();
        while (q != nullptr) {
            if (q->getData() < min->getData()) {
                min = q;
            }
            q = q->getNext();
        }
        if (min != p) {
            swapData(p, min);
        }
        p = p->getNext();
    }
}

template <class T>
T ListSimple<T>::retrieve(Node* p){
    if(!isValidPos(p)){
        throw Exception("Posicion invalida, retrieve");
    }

    return p->getData();
}

template <class T>
std::string ListSimple<T>::toString() const {
    Node* aux(anchor);
    std::string result;

    while(aux != nullptr){
        result+=aux->getData().toString() + "\n";

        aux = aux->getNext();
    }

    return result;
}

template <class T>
void ListSimple<T>::readFromDisk(const std::string& fileName) {
    std::ifstream myFile;
    T myData;

    myFile.open(fileName);

    if (!myFile.is_open()) {
        throw Exception("No se pudo abrir el archivo para lectura");
    }

    // Eliminar todos los elementos antes de cargar desde el archivo
    deleteAll();

    Node* lastNode = nullptr;

    while (myFile >> myData) {
        Node* newNode = new Node(myData);
        if (anchor == nullptr) {
            anchor = newNode;
        } else {
            lastNode->setNext(newNode);
        }
        lastNode = newNode;
    }

    myFile.close();
}

template <class T>
void ListSimple<T>::writeToDisk(const std::string& fileName) {
    std::ofstream myFile;

    myFile.open(fileName, std::ios_base::trunc);

    if (!myFile.is_open()) {
        throw Exception("No se pudo abrir el archivo para escritura");
    }

    Node* currentNode = anchor;
    while (currentNode != nullptr) {
        myFile << currentNode->getData() << std::endl;
        currentNode = currentNode->getNext();
    }

    myFile.close();
}


template <class T>
void ListSimple<T>::deleteAll() {
    Node* aux;

    while(anchor != nullptr) {
        aux = anchor;

        anchor = anchor->getNext();

        delete aux;
    }
}

template <class T>
ListSimple<T>& ListSimple<T>::operator = (const ListSimple<T>& l){
    deleteAll();

    copyAll(l);

    return *this;
}

#endif