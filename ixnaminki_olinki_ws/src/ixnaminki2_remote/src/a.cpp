#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        tcp::endpoint endpoint(tcp::v4(), 12345);
        tcp::acceptor acceptor(io_context, endpoint);

        std::cout << "Servidor esperando conexiones en el puerto 12345..." << std::endl;

        tcp::socket socket(io_context);
        acceptor.accept(socket);

        std::cout << "Cliente conectado!" << std::endl;

        for (;;) {
            std::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break; // Conexión cerrada limpiamente por el cliente
            else if (error)
                throw boost::system::system_error(error); // Otro error

            std::cout.write(buf.data(), len);
        }
    } catch (std::exception& e) {
        std::cerr << "Excepción: " << e.what() << std::endl;
    }

    return 0;
}
