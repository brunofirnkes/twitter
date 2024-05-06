CXX = g++
CXXFLAGS = -pthread -std=c++11 -Wall -Wextra

# List your source files for server and client
SERVER_SRCS = server/serverapp.cpp server/server.cpp
CLIENT_SRCS = client/clientapp.cpp client/client.cpp 

# Generate the names of object files
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)

# The target executables
SERVER_TARGET = server/server
CLIENT_TARGET = client/client

.PHONY: all clean

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS) $(SERVER_TARGET) $(CLIENT_TARGET)
