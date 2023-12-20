#include "DecoupledIO.hpp"

SC_MODULE(Producer)
{
    sc_vector<sc_port<IO_out_if<int>>> outs;
    sc_in<bool> clock;

    void produce()
    {
        int data = 0;
        while (true)
        {
            wait(clock->posedge_event());
            for (size_t i = 0; i < outs.size(); ++i)
            {
                if (!outs[i]->is_valid())
                {
                    outs[i]->write(data);
                    outs[i]->set_valid(true);
                    cout << "Produced " << data << " to consumer " << i << " at " << sc_time_stamp() << "," << sc_delta_count_at_current_time() << "\n";
                }
                else if (!outs[i]->is_ready())
                {
                }
                else
                {
                    outs[i]->write(data);
                    outs[i]->set_valid(true);
                    cout << "Produced " << data << " to consumer " << i << " at " << sc_time_stamp() << "," << sc_delta_count_at_current_time() << "\n";
                }
                data++;
            }
        }
    }

    SC_CTOR(Producer) : outs("outs")
    {
        SC_THREAD(produce);
    }
};

SC_MODULE(Consumer)
{
    sc_port<IO_in_if<int>> in;
    sc_in<bool> clock;
    int id; // Consumer的编号

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
                cout << "Consumer " << id << " set ready=true at " << sc_time_stamp() << "," << sc_delta_count_at_current_time() << "\n";
            }
            else
            {
                in->set_ready(false);
            }

            // 接受数据部分
            if (in->is_ready() && in->is_valid())
            {
                in->read(data);
                cout << "Consumer " << id << " consumed: " << data << " at " << sc_time_stamp() << "," << sc_delta_count_at_current_time() << "\n";
            }
        }
    }

    // 添加一个构造函数来设置编号
    SC_HAS_PROCESS(Consumer);
    Consumer(sc_module_name name) : sc_module(name), in("in")
    {
        SC_THREAD(consume);
    }
};

int sc_main(int argc, char *argv[])
{
    sc_clock clk("clk", 10, SC_NS);
    Producer prod("prod");
    sc_vector<DecoupledIO<int>> channels("channels", 3); // 实例化3个通道
    sc_vector<Consumer> consumers("consumers", 3);       // 实例化3个消费者

    // 连接生产者和通道
    prod.outs.init(channels.size());
    for (size_t i = 0; i < channels.size(); ++i)
    {
        prod.outs[i](channels[i]);
    }

    // 连接通道和消费者，并传递编号
    for (size_t i = 0; i < consumers.size(); ++i)
    {
        // 修改每个消费者模块，添加编号
        consumers[i].id = i;
        consumers[i].in(channels[i]);
        consumers[i].clock(clk);
    }

    prod.clock(clk);

    sc_start(100, SC_NS);
    return 0;
}
