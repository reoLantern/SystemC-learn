#include "DecoupledIO.hpp"


SC_MODULE(Producer)
{
    sc_port<IO_out_if<int>> out;
    sc_in<bool> clock;

    void produce()
    {
        int data = 0;
        while (true)
        {
            wait(clock->posedge_event());
            if (!out->is_valid())
            {
                out->write(data);
                out->set_valid(true);
                cout << "Produced " << data << " at " << sc_time_stamp() << "," << sc_delta_count_at_current_time() << "\n";
                data++;
            }
            else if (!out->is_ready())
            {
            }
            else
            {
                out->write(data);
                out->set_valid(true);
                cout << "Produced: " << data << " at " << sc_time_stamp() << "," << sc_delta_count_at_current_time() << "\n";
                data++;
            }
        }
    }

    SC_CTOR(Producer)
    {
        SC_THREAD(produce);
    }
};

SC_MODULE(Consumer)
{
    sc_port<IO_in_if<int>> in;
    sc_in<bool> clock;

    void consume()
    {
        int data;
        while (true)
        {
            wait(clock->posedge_event());

            // 控制ready部分
            if (sc_time_stamp().value() % 30 == 0 || sc_time_stamp().value() % 30 == 10)
            {
                in->set_ready(true);
                cout << "Consumer set ready=true at " << sc_time_stamp() << "," << sc_delta_count_at_current_time() << "\n";
            }
            else
            {
                in->set_ready(false);
            }

            // 接受数据部分
            if (in->is_ready() && in->is_valid())
            {
                in->read(data);
                cout << "Consumed: " << data << " at " << sc_time_stamp() << "," << sc_delta_count_at_current_time() << "\n";
            }
        }
    }

    SC_CTOR(Consumer)
    {
        SC_THREAD(consume);
    }
};

int sc_main(int argc, char *argv[])
{
    sc_clock clk("clk", 10, SC_NS); // cycle=10
    DecoupledIO<int> channel("channel");
    Producer prod("prod");
    Consumer cons("cons");

    prod.out(channel);
    cons.in(channel);

    prod.clock(clk);
    cons.clock(clk);

    sc_start(100, SC_NS);
    return 0;
}
