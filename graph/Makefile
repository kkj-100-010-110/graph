NAME		=	main
CXX			=	c++
CXXFLAGS	=	-std=c++20 
RM			=	rm -rf
SRCS		=	main.cpp \
				graph.cpp
OBJS		=	$(SRCS:%.cpp=%.o)
all		: $(NAME)
%.o		: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<
$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
clean	:
	$(RM) $(OBJS)
fclean	:	clean
	$(RM) $(NAME)
	rm Kruskal.dot Prim.dot Dijkstra.dot Kruskal.png Prim.png Dijkstra.png
re		:	fclean all
.PHONY	:	all clean fclean re