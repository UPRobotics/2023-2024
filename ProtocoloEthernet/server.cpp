#include <iostream>
#include <boost/asio.hpp>
#include <iomanip>

using boost::asio::ip::tcp;

float dneppa(uint32_t x1,uint32_t x2,uint32_t x3,uint32_t x4){
    return (float)((x1<<24) + (x2<<16) + (x3<<8) + x4)/1000.0;
}
float f(std::string s, int index){
    return dneppa(s[index], s[index+1], s[index+2], s[index+3]);
}
int main() {
    try {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 5000));

        std::cout << "Conectando" << std::endl;

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
            std::string s = buf.data();
            if(s.size()==0)  return;
            std::cout<<"\n------------------------------------------------\n";
            if(s.size()==28){
                std::cout<<"DATOS DE LA BMS\n"<<
                            "Voltaje de referencia: "<< std::fixed << std::setprecision(4) << f(s,0)<<"\n"; 
                for(int i = 1; i < 6; i++){
                    std::cout<<"Voltaje celda: "<<f(s, i*4)<<"\n";
                }
                std::cout<<"Corriente: "<<f(s, 24)<<"\n";
                
            }else{
                std::cout<<"STEPERS\n";
                if(f(s, 0)==3){
                    std::cout<<"El motor ha sido detenido\n";
                    return;
                }
                std::cout<<"El motor activo es: "<< std::fixed << std::setprecision(4) <<f(s,0)<<"\n"<<
                            "Posicion actual: "<< std::fixed << std::setprecision(4) <<f(s, 4)<<"\n"<<
                            "Velocidad: "<< std::fixed << std::setprecision(4) <<f(s, 8)<<"\n"<<
                            "Posicion deseada"<< std::fixed << std::setprecision(4) <<f(s, 12)<<"\n";
                for(int i = 12; i < s.size(); i++){
                    std::cout<<s[i];
                }
                std::cout<<"\n";
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Excepción: " << e.what() << std::endl;
    }

    return 0;
}