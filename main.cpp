#include <string>
#include <exception>
#include <mutex>
#include <iostream>
#include <array>

template<typename T, std::size_t capacity>
class circular_buffer
{
    public:
        circular_buffer()
            : size(0)
            , head(0)
            , tail(0)
        {}

        void put(T item)
        {
            std::lock_guard<std::mutex> lock(m);
            buf[head] = item;

            if (size++ == capacity)
            {
                tail = (tail + 1) % capacity;
            }

            head = (head + 1) % capacity;

            std::cout << "put head: " << head << " tail: " << tail << std::endl;
        }

        T get()
        {
            std::lock_guard<std::mutex> lock(m);
            if (size == 0)
            {
                throw std::runtime_error("Buffer is empty!");
            }

            auto val = buf[tail];
            size--;
            tail = (tail + 1) % capacity;

            std::cout << "get head: " << head << " tail: " << tail << std::endl;

            return val;
        }

        const T& operator[] (std::size_t i)
        {
            auto index = (tail + i) % capacity;
            if ( i < 0 || index > head || size == 0)
            {                
                throw std::invalid_argument("Index out of boundaries");
            }

            std::cout << "[] index: " << index << " head: " << head << " tail: " << tail << std::endl;

            return buf[index];
        }

    private:
        std::array<T, capacity> buf;

        std::mutex m;
        std::size_t size;
        std::size_t head;
        std::size_t tail;
};

int main()
{
    std::cout << "Hi there!" << std::endl;

    circular_buffer<std::string, 5> buf;
    buf.put("0");
    buf.put("1");
    buf.put("2");    
    std::cout << buf.get() << '\n';
    buf[1];
    buf.put("3");
    buf.get();
    buf.get();
    buf.get();
    buf[1];
    
    return 0;
}