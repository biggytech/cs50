select name from people
where id in (
    select distinct(people.id) from movies
    join directors on movies.id = directors.movie_id
    join people on directors.person_id = people.id
    join ratings on movies.id = ratings.movie_id
    where ratings.rating >= 9.0
);