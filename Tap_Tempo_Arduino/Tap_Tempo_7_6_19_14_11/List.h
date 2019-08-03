template <typename T> class List
{
  public:
    List();
    List(int init_capacity);

    T Get(int index);
    int GetSize();
    int GetCapacity();

    void Insert(T new_value, int insert_index);
    void Insert(T new_value);
    void Replace(T new_value, int replace_index);
//    void Remove(T remove_value);
//    void Remove(int index);
    void Reset();
    void Resize(int new_size);
    
  private:
    T *data;
    int size, capacity;
};

template <typename T> List<T>::List()
{
  size = 0;
  capacity = 0;
  data = 0;
}

template <typename T> List<T>::List(int init_capacity)
{
  if (init_capacity > 0)
  {
    size = 0;
    capacity = init_capacity;

    data = new T[capacity];
  }
}

template <typename T> T List<T>::Get(int index)
{
  if (index < 0)
  {
    index += size;
  }

  return data[index];
}
template <typename T> int List<T>::GetSize() { return size; }
template <typename T> int List<T>::GetCapacity() { return capacity; }

template <typename T> void List<T>::Insert(T new_value, int insert_index)
{
  if (insert_index < 0 || insert_index > size)
  {
    return;
  }
  
  if (size == capacity)
  {
    Resize(2*size);
  }

  for (int index = size; index > insert_index; index++)
  {
    data[index] = data[index-1];
  }

  data[insert_index] = new_value;
  size++;
}

template <typename T> void List<T>::Insert(T new_value)
{
  Insert(new_value, size);
}

template <typename T> void List<T>::Replace(T new_value, int replace_index)
{
  if (replace_index < 0 || replace_index >= size)
  {
    return;
  }

  data[replace_index] = new_value;
}
template <typename T> void List<T>::Reset()
{
  size = 0;
}

template <typename T> void List<T>::Resize(int new_size)
{
  if (new_size > 0 && new_size != size)
  {
    int new_capacity = 2 * new_size;
    T *new_data = new T[new_capacity];

    for (int index = 0; index < ((size < new_size) ? size : new_size); index++)
    {
      new_data[index] = data[index];
    }

    delete[] data;
    data = new_data;

    size = new_size;
    capacity = new_capacity;
  }
}
