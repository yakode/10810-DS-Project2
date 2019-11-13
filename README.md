# 10810-DS-Project2
Project2 -- Floor Cleaning Robot @Data Structures by RST

- bool all_clean();
</br>check if there is no point need to clean

- void cal_step_map(Point start);
</br> calculate the steps from Point start to every point
</br> steps are saved in step_map[1005][1005]

- Point find_farthest();
</br>return position of the unclean farthest point

- int step2R(Point start);
</br>return how many step needed to back to R

- int A2B(Point A, Point B, bool out);
</br>path of A to B
</br>if out == 1, output the path