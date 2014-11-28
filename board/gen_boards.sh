
#!/usr/bin/env bash

#aruco_create_board 3:2 b3x2.png b3x2.yml 128 1
#aruco_create_board 6:4 b6x4.png b3x2.yml 128

function create_board {
	aruco_create_board $1:$2 $3.png $3.yml $4 $5 $6
}

create_board 3 2 b3x2
create_board 6 4 b6x4

