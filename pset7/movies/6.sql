SELECT AVG(rating) from ratings where movie_id in (SELECT id from movies WHERE year = 2012);