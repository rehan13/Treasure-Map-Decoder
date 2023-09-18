/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

template <class T>
Deque<T>::Deque(){

/* YOUR CODE HERE! */
    data = vector<T>();
    n1 = 0;
    n2 = -1;

}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem)
{
    /**
     * @todo Your code here!
     */
    data.push_back(newItem);
    n2++;
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
    /**
     * @todo Your code here! 
     */
    

    T temp1 = data[n1];
    n1++;

    if ((n2 - n1) + 1 <= (n1)) {
        vector<T> temp = vector<T>();
        for (int i = n1; i <= n2; i++) {
            temp.push_back(data[i]);
        }
        n2 = (n2 - n1);
        n1 = 0;
        
        data = temp;
    }

    
    return temp1;

}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    /**
     * @todo Your code here! You will need to replace the following line.
     */
    

    T temp1 = data[n2];
    n2--;
    data.pop_back();

    if ((n2 - n1) + 1 <= (n1)) {
        vector<T> temp = vector<T>();
        for (int i = n1; i <= n2; i++) {
            temp.push_back(data[i]);
        }
        n2 = (n2 - n1);
        n1 = 0;
        
        data = temp;
    }

    return temp1;
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    /**
     * @todo Your code here! 
     */
    return data[n1];
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    /**
     * @todo Your code here! 
     */
    return data[n2];
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    /**
     * @todo Your code here! 
     *      */
    return (n2 < n1);
}
