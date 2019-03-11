#include <exception>
#include <mutex>
#include <iostream>
#include <array>

template<typename T, std::size_t capasity>
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

            if (size++ == capasity)
            {
                tail = (tail + 1) % capasity;
            }

            head = (head + 1) % capasity;
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
            tail = (tail + 1) % capasity;

            return val;
        }

    private:
        std::array<T, capasity> buf;

        std::mutex m;
        std::size_t size;
        std::size_t head;
        std::size_t tail;
};

int main()
{
    std::cout << "Hi there!" << std::endl;

    circular_buffer<int, 3> buf;
    buf.put(0);
    buf.put(1);
    buf.put(2);
    std::cout << "Pop: " << buf.get() << std::endl;
    buf.put(3);
    std::cout << "Pop: " << buf.get() << std::endl;
    std::cout << "Pop: " << buf.get() << std::endl;
    std::cout << "Pop: " << buf.get() << std::endl;


    return 0;
}